// MIT License
//
// Copyright (c) 2020 Xu Collaborators
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Tessellation.hpp"

#include <mapbox/earcut.hpp>

#include <optional>
#include <cassert>
#include <array>

namespace xu {

// All of the following quadratic-related functions have been taken from:
// https://raphlinus.github.io/graphics/curves/2019/12/23/flatten-quadbez.html

struct BasicQuadBez {
    float x0;
    float x2;
    float scale;
    float cross;
};

struct QuadBez {
    FPoint2 p0;
    FPoint2 p1;
    FPoint2 p2;
};

static BasicQuadBez MapToBasic(QuadBez const& bez) {
    const auto ddx = 2 * bez.p1.x - bez.p0.x - bez.p2.x;
    const auto ddy = 2 * bez.p1.y - bez.p0.y - bez.p2.y;
    const auto u0 = (bez.p1.x - bez.p0.x) * ddx + (bez.p1.y - bez.p0.y) * ddy;
    const auto u2 = (bez.p2.x - bez.p1.x) * ddx + (bez.p2.y - bez.p1.y) * ddy;
    const auto cross
        = (bez.p2.x - bez.p0.x) * ddy - (bez.p2.y - bez.p0.y) * ddx;
    const auto x0 = u0 / cross;
    const auto x2 = u2 / cross;
    const auto scale
        = std::abs(cross) / (std::hypot(ddx, ddy) * std::abs(x2 - x0));
    return BasicQuadBez{x0, x2, scale, cross};
}

static float ApproxMyint(float x) {
    const auto d = 0.67f;
    return x / (1 - d + std::pow(std::pow(d, 4.f) + 0.25 * x * x, 0.25));
}

static float ApproxInvMyint(float x) {
    const auto b = 0.39;
    return x * (1.f - b + std::sqrt(b * b + 0.25f * x * x));
}

static FPoint2 EvaluateQuadraticBez(QuadBez const& bez, float t) {
    const auto mt = 1.f - t;
    const auto x
        = bez.p0.x * mt * mt + 2 * bez.p1.x * t * mt + bez.p2.x * t * t;
    const auto y
        = bez.p0.y * mt * mt + 2 * bez.p1.y * t * mt + bez.p2.y * t * t;
    return FPoint2{x, y};
}

static std::vector<FPoint2> FlattenQuadratic(
    FPoint2 from, FPoint2 to, FPoint2 p0, double quality) {
    const auto bez = QuadBez{from, p0, to};
    const auto tol = 1.f / quality;
    const auto params = MapToBasic(bez);
    const auto a0 = ApproxMyint(params.x0);
    const auto a2 = ApproxMyint(params.x2);
    const auto count = 0.5 * std::abs(a2 - a0) * std::sqrt(params.scale / tol);
    const auto n = std::ceil(count);
    const auto u0 = ApproxInvMyint(a0);
    const auto u2 = ApproxInvMyint(a2);

    std::vector<float> result;
    result.push_back(0.f);
    if (std::isfinite(n)) {
        result.reserve(n + 2);
        for (std::size_t i = 1; i < n; ++i) {
            const auto u = ApproxInvMyint(a0 + ((a2 - a0) * i) / n);
            const auto t = (u - u0) / (u2 - u0);
            result.push_back(t);
        }
    }
    result.push_back(1.f);

    std::vector<FPoint2> points;
    points.reserve(result.size());
    for (auto const& t : result) {
        points.push_back(EvaluateQuadraticBez(bez, t));
    }
    return points;
}

// https://gist.github.com/rlindsay/c55be560ec41144f521f
static std::vector<FPoint2> FlattenCubic(
    FPoint2 p1, FPoint2 p2, FPoint2 p3, FPoint2 p4, double quality) {
    const auto numLines
        = (static_cast<std::size_t>((p4 - p1).Magnitude() * 2)) + 1;
    std::vector<FPoint2> points;
    points.resize(numLines);

    const float cx = 3 * (p2.x - p1.x);
    const float cy = 3 * (p2.y - p1.y);
    const float bx = 3 * (p3.x - p2.x) - cx;
    const float by = 3 * (p3.y - p2.y) - cy;
    const float ax = p4.x - p1.x - cx - bx;
    const float ay = p4.y - p1.y - cy - by;
    const float h = 1.f / numLines;
    const float hh = h * h;
    const float hhh = hh * h;
    // TODO: optimize this
    float d1x = ax * hhh + bx * hh + cx * h;
    float d1y = ay * hhh + by * hh + cy * h;
    float d2x = 6 * ax * hhh + 2 * bx * hh;
    float d2y = 6 * ay * hhh + 2 * by * hh;
    float d3x = 6 * ax * hhh;
    float d3y = 6 * ay * hhh;

    FPoint2 curr = p1;
    points[0] = curr;
    for (auto& point : points) {
        curr.x += d1x;
        curr.y += d1y;

        d1x += d2x;
        d1y += d2y;

        d2x += d3x;
        d2y += d3y;

        point = curr;
    }
    points.back() = p4;

    return points;
}

static float Lerp(float a, float b, float f) { return a + f * (b - a); }

static FPoint2 PointForAngle(float radians, FPoint2 center, float radius) {
    const float s = std::sinf(radians);
    const float c = std::cosf(radians);
    return FPoint2{c * radius + center.x, s * radius + center.y};
}

static std::vector<FPoint2> FlattenArc(FPoint2 center, float radius,
    float startAngle, float endAngle, double quality) {
    std::vector<FPoint2> polygon;
    for (size_t i = 0; i < std::ceil(quality); ++i) {
        float t = i / static_cast<float>(std::ceil(quality));
        polygon.push_back(
            PointForAngle(Lerp(startAngle, endAngle, t), center, radius));
    }
    return polygon;
}

static std::vector<FPoint2> MergeDuplicatePoints(
    std::vector<FPoint2> const& polygon) {
    std::vector<FPoint2> merged;
    for (auto const& point : polygon) {
        if (merged.size() > 0 && merged.back() == point) { continue; }
        merged.push_back(point);
    }
    return merged;
}

std::vector<FPoint2> FlattenPath(VectorPath const& path, double quality) {
    std::vector<FPoint2> polygon;

    FPoint2 curr = path.start;
    for (auto const& event : path.events) {
        switch (event.type) {
            case VectorPathEventType::Line: {
                if (polygon.empty() || polygon.back() != curr)
                    polygon.push_back(curr);
                curr = event.params.line.to;
                polygon.push_back(curr);
                break;
            }
            case VectorPathEventType::Quadratic: {
                const auto points
                    = FlattenQuadratic(curr, event.params.quadratic.to,
                        event.params.quadratic.x0, quality);

                if (points.empty()) continue;
                // back() is invalid on an empty vector, so we need to make this
                // distinction
                if (polygon.empty()) {
                    polygon.insert(polygon.end(), points.begin(), points.end());
                } else {
                    polygon.insert(polygon.end(),
                        points.begin()
                            + (points.front() == polygon.back() ? 1 : 0),
                        points.end());
                }
                curr = points.back();
                break;
            }
            case VectorPathEventType::Cubic: {
                const auto points = FlattenCubic(curr, event.params.cubic.to,
                    event.params.cubic.x0, event.params.cubic.x1, quality);

                if (points.empty()) continue;
                // back() is invalid on an empty vector, so we need to make this
                // distinction
                if (polygon.empty()) {
                    polygon.insert(polygon.end(), points.begin(), points.end());
                } else {
                    polygon.insert(polygon.end(),
                        points.begin()
                            + (points.front() == polygon.back() ? 1 : 0),
                        points.end());
                }
                curr = points.back();
                break;
            }
            case VectorPathEventType::Arc: {
                const auto points = FlattenArc(event.params.arc.center,
                    event.params.arc.radius, event.params.arc.startAngle,
                    event.params.arc.endAngle, quality);

                if (points.empty()) continue;
                // back() is invalid on an empty vector, so we need to make this
                // distinction
                if (polygon.empty()) {
                    polygon.insert(polygon.end(), points.begin(), points.end());
                } else {
                    polygon.insert(polygon.end(),
                        points.begin()
                            + (points.front() == polygon.back() ? 1 : 0),
                        points.end());
                }
                curr = points.back();
                break;
            }
        }
    }

    return polygon;
}

inline static FVector2 PolylineDirection(FPoint2 a, FPoint2 b) {
    return (a - b).Normalized();
}

inline static FVector2 PolylineNormal(FVector2 direction) {
    return FVector2{-direction.y, direction.x};
}

static void PolylineExtrusions(
    std::vector<FPoint2>& points, FPoint2 point, FVector2 normal, float scale) {
    points.push_back(point + (normal * -scale));
    points.push_back(point + (normal * scale));
}

static float PolylineComputeMiter(FVector2& tangent, FVector2 miter,
    FPoint2 lineA, FPoint2 lineB, float halfThick) {
    tangent = lineA + lineB;
    tangent = tangent.Normalized();
    miter = {-tangent.y, tangent.x};
    const FVector2 tmp{-lineA.y, lineA.x};
    return halfThick / ((miter.x * tmp.x) + (miter.y * tmp.y));
}

struct PolylineExpansion {
    FVector2 tmp{0.f, 0.f};
    FVector2 capEnd{0.f, 0.f};
    FVector2 lineA{0.f, 0.f};
    FVector2 lineB{0.f, 0.f};
    FVector2 tangent{0.f, 0.f};
    FVector2 miter{0.f, 0.f};

    float miterLimit;
    float thickness;
    LineJoin join;
    LineCap cap;

    std::optional<FVector2> normal;
    int lastFlip;
    bool started;

    auto Build(std::vector<FPoint2> const& points) {
        std::pair<std::vector<FPoint2>, std::vector<uint32_t>> output;

        if (points.size() <= 1) return output;

        lastFlip = -1;
        started = false;
        normal = std::nullopt;

        for (std::size_t i = 1, count = 0; i < points.size(); i++) {
            FVector2 last = points[i - 1];
            FVector2 cur = points[i];
            std::optional<FVector2> next = i < points.size() - 1
                ? std::optional<FVector2>{points[i + 1]}
                : std::nullopt;
            int amt = PolylineSegment(
                output, count, last, cur, next, thickness / 2);
            count += amt;
        }

        return output;
    }

    int PolylineSegment(
        std::pair<std::vector<FPoint2>, std::vector<uint32_t>>& output,
        int index, FVector2 last, FVector2 cur, std::optional<FVector2> next,
        float halfThick) {
        int count = 0;
        const bool capSquare = cap == LineCap::Square;
        const bool joinBevel = join == LineJoin::Bevel;

        lineA = PolylineDirection(cur, last);

        if (!normal) { normal = PolylineNormal(lineA); }

        if (!started) {
            started = true;

            if (capSquare) {
                capEnd = last + (lineA * -halfThick);
                last = capEnd;
            }

            PolylineExtrusions(output.first, last, normal.value(), halfThick);
        }

        output.second.push_back(index);
        output.second.push_back(index + 1);
        output.second.push_back(index + 2);

        if (!next) {
            normal = PolylineNormal(lineA);

            if (capSquare) {
                capEnd = cur + (lineA * halfThick);
                cur = capEnd;
            }

            PolylineExtrusions(output.first, cur, normal.value(), halfThick);

            if (lastFlip == 1) {
                output.second.push_back(index);
                output.second.push_back(index + 2);
                output.second.push_back(index + 3);
            } else {
                output.second.push_back(index + 2);
                output.second.push_back(index + 1);
                output.second.push_back(index + 3);
            }

            count += 2;
        } else {
            lineB = PolylineDirection(next.value(), cur);

            float miterLen
                = PolylineComputeMiter(tangent, miter, lineA, lineB, halfThick);

            int flip = ((tangent.x * normal->x) + (tangent.y * normal->y)) < 0
                ? -1
                : 1;

            bool bevel = joinBevel;
            if (!bevel && join == LineJoin::Miter) {
                float limit = miterLen / halfThick;
                if (limit > miterLimit) bevel = true;
            }

            if (bevel) {
                tmp = cur + (normal.value() * (-halfThick * flip));
                output.first.push_back(tmp);
                tmp = cur + (miter * (miterLen * flip));
                output.first.push_back(tmp);

                if (lastFlip != -flip) {
                    output.second.push_back(index);
                    output.second.push_back(index + 2);
                    output.second.push_back(index + 3);
                } else {
                    output.second.push_back(index + 2);
                    output.second.push_back(index + 1);
                    output.second.push_back(index + 3);
                }

                output.second.push_back(index + 2);
                output.second.push_back(index + 3);
                output.second.push_back(index + 4);

                tmp = PolylineNormal(lineB);
                normal = tmp;

                tmp = cur + (tmp * (-halfThick * flip));
                output.first.push_back(tmp);

                count += 3;
            } else {
                PolylineExtrusions(output.first, cur, miter, miterLen);

                if (lastFlip == 1) {
                    output.second.push_back(index);
                    output.second.push_back(index + 2);
                    output.second.push_back(index + 3);
                } else {
                    output.second.push_back(index + 2);
                    output.second.push_back(index + 1);
                    output.second.push_back(index + 3);
                }

                flip = -1;

                normal = miter;
                count += 2;
            }
            lastFlip = flip;
        }

        return count;
    }
};

// Taken from:
// https://github.com/mattdesl/extrude-polyline
std::pair<std::vector<FPoint2>, std::vector<uint32_t>> ExpandStroke(
    std::vector<FPoint2> const& polygon, const float strokeWidth,
    const LineCap cap, const LineJoin join, const float miterLimit,
    const double quality) {
    PolylineExpansion e;
    e.miterLimit = miterLimit;
    e.thickness = strokeWidth;
    e.join = join;
    e.cap = cap;
    return e.Build(polygon);
}

std::vector<uint32_t> Triangulate(std::vector<FPoint2> const& polygon) {
    if (polygon.size() <= 2) { return {}; }

    using Point = std::array<float, 2>;
    std::vector<std::vector<Point>> x{{}};
    x[0].resize(polygon.size());
    for (std::size_t i = 0; i < polygon.size(); ++i) {
        x[0][i] = {polygon[i].x, polygon[i].y};
    }
    return mapbox::earcut<uint32_t>(x);
}

} // namespace xu
