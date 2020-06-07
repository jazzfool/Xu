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

#pragma once

#include <xu/core/Definitions.hpp>
#include <xu/core/Size2.hpp>

#include <array>
#include <vector>

namespace xu {

enum class XU_API VectorPathEventType { Line, Quadratic, Cubic, Arc };

struct XU_API VectorPathEvent {
    static VectorPathEvent Line(FPoint2 to);
    static VectorPathEvent Quadratic(FPoint2 to, FPoint2 x0);
    static VectorPathEvent Cubic(FPoint2 to, FPoint2 x0, FPoint2 x1);
    static VectorPathEvent Arc(
        FPoint2 center, float radius, float startAngle, float endAngle);

    VectorPathEventType type;
    union XU_API {
        struct XU_API {
            FPoint2 to;
        } line;
        struct XU_API {
            FPoint2 to;
            FPoint2 x0;
        } quadratic;
        struct XU_API {
            FPoint2 to;
            FPoint2 x0;
            FPoint2 x1;
        } cubic;
        struct XU_API {
            FPoint2 center;
            float radius;
            float startAngle;
            float endAngle;
        } arc;
    } params;
};

enum class LineCap { Butt, Square };
enum class LineJoin { Bevel, Miter };

struct XU_API BakedVectorPath {
    BakedVectorPath WithOffset(FVector2 offset) const;
    BakedVectorPath WithScale(FVector2 scale) const;

    const std::vector<FPoint2> vertices;
    const std::vector<uint32_t> indices;
};

struct XU_API VectorPath {
    static VectorPath Rectangle(FSize2 size);
    static VectorPath RoundRectangle(
        FSize2 size, std::array<float, 4> cornerRadii);
    static VectorPath RoundRectangle(FSize2 size, float cornerRadius);
    static VectorPath HorizontalLine(float length);
    static VectorPath VerticaLine(float length);
    static VectorPath Circle(FVector2 radii);

    BakedVectorPath BakeFill(double quality) const;
    BakedVectorPath BakeStroke(double quality, float strokeWidth,
        LineCap cap = LineCap::Butt, LineJoin join = LineJoin::Miter,
        float miterLimit = 1.f) const;

    FPoint2 start{0.f, 0.f};
    std::vector<VectorPathEvent> events;
};

} // namespace xu