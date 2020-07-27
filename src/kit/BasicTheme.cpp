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

#include "xu/core/Theme.hpp"
#include <xu/kit/BasicTheme.hpp>
#include <xu/kit/Button.hpp>

namespace xu {

BasicTheme::BasicTheme() {
    params.normalTextSize = 14.0;
    params.headingTextSize = 28.0;
}

Color BasicTheme::ColorFromPalette(const std::string& colorName) {
    if (colorName == Theme::foregroundTextColor) {
        return Color::White();
    } else if (colorName == Theme::backgroundColor) {
        return Color::Black();
    } else if (colorName == Theme::outlineColor) {
        return Color::White();
    } else {
        return Color::Black();
    }
}

void BasicTheme::InitializeWidget(
    Widget* widget, PaintInfo* info, PainterType basePainter) {
    if (basePainter == typeid(Button)) {
        Button* btn = static_cast<Button*>(widget);
        info->paths = std::move(std::vector{
            VectorPath::Rectangle(btn->Geometry().size).BakeFill(0.f)});
    }
}

void BasicTheme::PaintWidget(Surface& surf, Widget const* widget,
    PaintInfo const* info, PainterType basePainter) {
    if (basePainter == typeid(Button)) {
        for (auto const& path : info->paths) {
            surf.Paint(path, Color::White());
        }
    }
}

Parameters const& BasicTheme::GetParameters() const { return params; }

} // namespace xu
