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

#include <algorithm>
#include <assert.h>
#include <xu/core/Definitions.hpp>
#include <initializer_list>
#include <xu/core/Point2.hpp>
#include <type_traits>

namespace xu {
// 2D rectangle type with helper methods.
template<typename T>
struct XU_API Bounds2 {
    Point2<T> lower, upper;

    constexpr Bounds2() = default;
    constexpr Bounds2(T left, T right, T top, T bottom) :
        lower(left, top),
        upper(right, bottom) {}
    constexpr Bounds2(const Point2<T>& lower, const Point2<T>& upper) :
        lower(lower),
        upper(upper) {}
    constexpr Bounds2(const std::initializer_list<Point2<T>>& vertices) :
        lower(std::numeric_limits<T>::max(), std::numeric_limits<T>::max()),
        upper(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()) 
    {
        for (const auto& v : vertices) {
            lower.x = std::min(v.x, lower.x);
            lower.y = std::min(v.y, lower.y);
            upper.x = std::max(v.x, upper.x);
            upper.y = std::max(v.y, upper.y);
        }
    }

    constexpr bool ContainsPoint(const Point2<T>& p) const {
        return p >= lower && p <= upper;
    }

    constexpr bool Overlaps(const Bounds2& other) const {
        if (upper.x < other.lower.x || lower.x > other.upper.x) return false;
        if (upper.y < other.lower.y || lower.y > other.upper.y) return false;

        return true;
    }

    constexpr bool FullyContains(const Bounds2& other) const {
        return lower <= other.lower && upper >= other.upper;
    }

    // Returns a new Bounds2<T> with the size of (size * (1 + factor))
    constexpr Bounds2<T> Fattened(float factor) const {
        if constexpr (std::is_signed_v<T>)
            assert(factor >= -1.f); // Attempts to fatten past -1 will result in
                                    // an invalid bounding box

        Vector2<T> deltaSize;
        factor /= 2.f; // Div by two to properly follow the equation size = size
                       // + size*factor

        // Overload for floating points to make sure factor is the same type as
        // T
        if constexpr (std::is_floating_point_v<T>)
            deltaSize = (upper - lower) * static_cast<T>(factor);
        else // If T is not floating point, ensure that multipication is proper
             // and then truncate down
            deltaSize = static_cast<Vector2<T>>(
                static_cast<FVector2>((upper - lower)) * factor);

        return Bounds2<T>(lower - deltaSize, upper + deltaSize);
    }

    constexpr Vector2<T> Size() const { return upper - lower; }

    constexpr T& Top() { return lower.y; }
    constexpr const T& Top() const { return lower.y; }

    constexpr T& Bottom() { return upper.y; }
    constexpr const T& Bottom() const { return upper.y; }

    constexpr T& Left() { return lower.x; }
    constexpr const T& Left() const { return lower.x; }

    constexpr T& Right() { return upper.x; }
    constexpr const T& Right() const { return upper.x; }

    bool operator==(const Bounds2<T>& rhs) const {
        return lower == rhs.lower && upper == rhs.upper;
    }

    bool operator!=(const Bounds2<T>& rhs) const { return !(*this == rhs); }

    template<typename Cast_t>
    constexpr operator Bounds2<Cast_t>() const {
        return Bounds2<Cast_t>(static_cast<Point2<Cast_t>>(lower),
            static_cast<Point2<Cast_t>>(upper));
    }
};

using IBounds2 = Bounds2<int>;
using FBounds2 = Bounds2<float>;

} // namespace xu
