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

#include <xu/modules/quick/DarculaTheme.hpp>

#include <xu/kit/Button.hpp>

namespace xu::quick {

static constexpr float CornerRadius = 10.f;

DarculaTheme::DarculaTheme() {
    params.normalTextSize = 12.f;
    params.headingTextSize = 26.f;
}

Color DarculaTheme::ColorFromPalette(std::string const& colorName) {
    if (colorName == Theme::foregroundTextColor) {
        return Color{230, 230, 230, 1.0f};
    } else if (colorName == Theme::backgroundColor) {
        return Color{50, 50, 50, 1.0f};
    } else if (colorName == Theme::outlineColor) {
        return Color{150, 150, 150, 1.0f};
    } else {
        return Color::Black();
    }
}

void DarculaTheme::InitializeWidget(
    Widget* widget, PaintInfo* info, PainterType basePainter) {
    if (basePainter == typeid(Button)) {
        Button* btn = static_cast<Button*>(widget);
        info->paths = std::move(std::vector{
            VectorPath::RoundRectangle(btn->Geometry().size, CornerRadius)
                .BakeStroke(50.f, 5.f, LineCap::Butt, LineJoin::Bevel)});
    }
}

void DarculaTheme::PaintWidget(Surface& surf, Widget const* widget,
    PaintInfo const* info, PainterType basePainter) {
    if (basePainter == typeid(Button)) {
        for (auto const& path : info->paths) {
            surf.Paint(
                path.WithOffset(widget->Geometry().origin), Color::White());
        }
    }
}

Parameters const& DarculaTheme::GetParameters() const { return params; }

} // namespace xu::quick
