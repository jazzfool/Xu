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

#include <xu/core/VectorPath.hpp>
#include "Tessellation.hpp"

namespace xu {

static const double PI = std::atan(1.0) * 4.;

VectorPathEvent VectorPathEvent::Line(FPoint2 to) {
    VectorPathEvent evt;
    evt.type = VectorPathEventType::Line;
    evt.params.line.to = to;
    return evt;
}

VectorPathEvent VectorPathEvent::Quadratic(FPoint2 to, FPoint2 x0) {
    VectorPathEvent evt;
    evt.type = VectorPathEventType::Quadratic;
    evt.params.quadratic.to = to;
    evt.params.quadratic.x0 = x0;
    return evt;
}

VectorPathEvent VectorPathEvent::Cubic(FPoint2 to, FPoint2 x0, FPoint2 x1) {
    VectorPathEvent evt;
    evt.type = VectorPathEventType::Cubic;
    evt.params.cubic.to = to;
    evt.params.cubic.x0 = x0;
    evt.params.cubic.x1 = x1;
    return evt;
}

VectorPathEvent VectorPathEvent::Arc(
    FPoint2 center, float radius, float startAngle, float endAngle) {
    VectorPathEvent evt;
    evt.type = VectorPathEventType::Arc;
    evt.params.arc.center = center;
    evt.params.arc.radius = radius;
    evt.params.arc.startAngle = startAngle;
    evt.params.arc.endAngle = endAngle;
    return evt;
}

BakedVectorPath BakedVectorPath::WithOffset(FVector2 const offset) const {
    auto newVerts = vertices;
    for (auto& vert : newVerts) { vert += offset; }
    return BakedVectorPath{newVerts, indices};
}

BakedVectorPath BakedVectorPath::WithScale(FVector2 const scale) const {
    auto newVerts = vertices;
    for (auto& vert : newVerts) { vert *= scale; }
    return BakedVectorPath{newVerts, indices};
}

VectorPath VectorPath::Rectangle(FSize2 const size) {
    VectorPath out;
    out.events = {VectorPathEvent::Line({size.x, 0.f}),
        VectorPathEvent::Line(size), VectorPathEvent::Line({0.f, size.y}),
        VectorPathEvent::Line({0.f, 0.f})};
    return out;
}

VectorPath VectorPath::RoundRectangle(
    FSize2 const size, std::array<float, 4> cornerRadii) {
    const float maxRadius = std::min(size.x, size.y) / 2.f;
    for (auto& cr : cornerRadii) { cr = std::min(cr, maxRadius); }

    VectorPath out;
    out.start.y = cornerRadii[0];
    out.events = {VectorPathEvent::Arc({cornerRadii[0], cornerRadii[0]},
                      cornerRadii[0], PI, 1.5 * PI),
        VectorPathEvent::Line({size.x - cornerRadii[1], 0.f}),
        VectorPathEvent::Arc({size.x - cornerRadii[1], cornerRadii[1]},
            cornerRadii[1], 1.5 * PI, 2. * PI),
        VectorPathEvent::Line({size.x, size.y - cornerRadii[2]}),
        VectorPathEvent::Arc({size.x - cornerRadii[2], size.y - cornerRadii[2]},
            cornerRadii[2], 0.f, 0.5 * PI),
        VectorPathEvent::Line({cornerRadii[3], size.y}),
        VectorPathEvent::Arc({cornerRadii[3], size.y - cornerRadii[3]},
            cornerRadii[3], 0.5 * PI, PI),
        VectorPathEvent::Line({0.f, cornerRadii[0]})};

    return out;
}

VectorPath VectorPath::RoundRectangle(
    FSize2 const size, float const cornerRadius) {
    return RoundRectangle(
        size, {cornerRadius, cornerRadius, cornerRadius, cornerRadius});
}

BakedVectorPath VectorPath::BakeFill(double quality) const {
    const auto vertices = FlattenPath(*this, quality);
    return BakedVectorPath{vertices, Triangulate(vertices)};
}

BakedVectorPath VectorPath::BakeStroke(double quality, float strokeWidth,
    LineCap cap = LineCap::Butt, LineJoin join = LineJoin::Miter,
    float miterLimit = 1.f) const {
    const auto flattened = FlattenPath(*this, quality);
    const auto [vertices, indices]
        = ExpandStroke(flattened, strokeWidth, cap, join, miterLimit, quality);
    return BakedVectorPath{vertices, indices};
}

} // namespace xu
