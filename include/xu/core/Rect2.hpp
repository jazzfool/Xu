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
#include <xu/core/Point2.hpp>
#include <xu/core/Bounds2.hpp>

namespace xu {

// 2D rectangle type with helper methods.
template<typename T>
struct XU_API Rect2 {
    Point2<T> origin;
    Vector2<T> size;

    constexpr Rect2() = default;
    constexpr Rect2(const Point2<T>& origin, const Vector2<T>& size) :
        origin(origin),
        size(size) {}
    constexpr Rect2(const Bounds2<T>& bounds) :
        origin(bounds.lower),
        size(bounds.Size()) {}

    constexpr bool ContainsPoint(const Point2<T>& p) const {
        return p >= origin && p <= (origin + size);
    }

    constexpr bool Overlaps(const Rect2<T>& other) const {
        return Bounds().Overlaps(other.Bounds());
    }

    constexpr bool FullyContains(const Rect2<T>& other) const {
        return Bounds().FullyContains(other.Bounds());
    }

    constexpr Bounds2<T> Bounds() const {
        return Bounds2<T>(origin, origin + size);
    }

    // Equality operators

    constexpr bool operator==(const Rect2& rhs) const {
        return origin == rhs.origin && size == rhs.size;
    }

    constexpr bool operator!=(const Rect2& rhs) const {
        return !(*this == rhs);
    }

    // Conversion operator
    template<typename Cast_t>
    constexpr operator Rect2<Cast_t>() const {
        return Rect2<Cast_t>(static_cast<Point2<Cast_t>>(origin),
            static_cast<Vector2<Cast_t>>(size));
    }
};

using IRect2 = Rect2<int>;
using FRect2 = Rect2<float>;

} // namespace xu
