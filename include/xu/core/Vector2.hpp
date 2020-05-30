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

#include "Definitions.hpp"

#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

namespace xu {

// 2D vector.
template <typename T> struct XU_API Vector2 {
  static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");

  T x, y;

  constexpr Vector2() noexcept = default;
  constexpr Vector2(T x, T y) noexcept : x(x), y(y) { };

  static constexpr Vector2 Zero() { return Vector2<T>(0, 0); }

  // Returns the size of the vector via pythagorean's theorem
  constexpr float Magnitude() const {
    return sqrtf(static_cast<float>(x*x + y*y));
  }

  // Returns the magnitude of the vector squared.
  constexpr float Magnitude2() const {
    return static_cast<float>(x*x + y*y);
  }

  // Returns a new vector pointing in the same direction but with a magnitude of 1
  Vector2<T> Normalized() const {
    return *this / Magnitude();
  }

  // Conversion operator
  template<typename Cast_t>
  explicit constexpr operator Vector2<Cast_t>() const {
    return Vector2<Cast_t>(static_cast<Cast_t>(x), static_cast<Cast_t>(y));
  }

  // Equality operators
  constexpr bool operator!=(const Vector2<T>& rhs) const { return !(*this == rhs); }
  constexpr bool operator==(const Vector2<T>& rhs) const {
    if constexpr (std::is_floating_point_v<T>) { // Epsilon equality for floating points
      return std::abs(x - rhs.x) <= std::numeric_limits<T>::epsilon()
          && std::abs(y - rhs.y) <= std::numeric_limits<T>::epsilon();
    } else {
      return x == rhs.x && y == rhs.y;
    }
  }

  // Negation operator
  Vector2<T> operator-() { auto temp = *this; return temp *= static_cast<T>(-1); }

  // Vector|Vector arithmetic assignment operators
  Vector2<T>& operator+=(const Vector2<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
  Vector2<T>& operator-=(const Vector2<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
  Vector2<T>& operator*=(const Vector2<T>& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
  Vector2<T>& operator/=(const Vector2<T>& rhs) { x /= rhs.x; y /= rhs.y; return *this; }

  // Vector|Scalar arithmetic assignment operators
  Vector2<T>& operator+=(T sclr) { return *this += Vector2<T>(sclr, sclr); }
  Vector2<T>& operator-=(T sclr) { return *this -= Vector2<T>(sclr, sclr); }
  Vector2<T>& operator*=(T sclr) { return *this *= Vector2<T>(sclr, sclr); }
  Vector2<T>& operator/=(T sclr) { return *this /= Vector2<T>(sclr, sclr); }

  // Vector|Vector arithmetic operators (defined in terms of += and friends)
  Vector2<T> operator+(const Vector2<T>& rhs) const { auto temp = *this; return temp += rhs; }
  Vector2<T> operator-(const Vector2<T>& rhs) const { auto temp = *this; return temp -= rhs; }
  Vector2<T> operator*(const Vector2<T>& rhs) const { auto temp = *this; return temp *= rhs; }
  Vector2<T> operator/(const Vector2<T>& rhs) const { auto temp = *this; return temp /= rhs; }

  // Vector|Scalar arithmetic operators (defined in terms of += and friends)
  Vector2<T> operator+(T sclr) const { auto temp = *this; return temp += sclr; } 
  Vector2<T> operator-(T sclr) const { auto temp = *this; return temp -= sclr; } 
  Vector2<T> operator*(T sclr) const { auto temp = *this; return temp *= sclr; } 
  Vector2<T> operator/(T sclr) const { auto temp = *this; return temp /= sclr; }

  // Friend Vector|Scalar arithmetic operators (so once can do, say 2 + Vector2<int>(3, 4) == Vector2<int>(5, 6))
  friend Vector2<T> operator+(T sclr, const Vector2<T>& rhs) { return rhs + sclr; } 
  friend Vector2<T> operator-(T sclr, const Vector2<T>& rhs) { return rhs - sclr; } 
  friend Vector2<T> operator*(T sclr, const Vector2<T>& rhs) { return rhs * sclr; } 
  friend Vector2<T> operator/(T sclr, const Vector2<T>& rhs) { return rhs / sclr; }
};

using IVector2 = Vector2<int>;
using FVector2 = Vector2<float>;

} // namespace xu
