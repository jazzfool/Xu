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

#include <string>
#include <typeindex>

#include <xu/core/Color.hpp>
#include <xu/core/Surface.hpp>

namespace xu {

class Widget;

struct XU_API PaintInfo {};

struct XU_API Parameters {
    float normalTextSize;
    float headingTextSize;
};

class XU_API Theme {
public:
    static constexpr char const* foregroundTextColor = "fgText";
    static constexpr char const* backgroundColor = "bg";
    static constexpr char const* outlineColor = "outline";

    virtual Color ColorFromPalette(std::string const& colorName) = 0;
    virtual void PaintWidget(Surface& surf, Widget const* widget,
        PaintInfo const* info, std::type_index basePainter)
        = 0;
    virtual Parameters const& GetParameters() const = 0;
};

} // namespace xu