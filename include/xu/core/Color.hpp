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

#include <array>
#include <xu/core/Definitions.hpp>

namespace xu {

/*!
 * \brief Simple RGBA 888+F (3x8-bit RGB components, 1x float alpha) color type.
 */
struct XU_API Color {
    static constexpr Color Black() { return Color{0, 0, 0, 1.f}; }
    static constexpr Color White() { return Color{255, 255, 255, 1.f}; }
    static constexpr Color Transparent() { return Color{0, 0, 0, 0.f}; }

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    float a = 1.0f;

    std::array<float, 4> Normalized() const {
        return std::array<float, 4>{{r / 255.0f, g / 255.0f, b / 255.0f, a}};
    }
};

} // namespace xu