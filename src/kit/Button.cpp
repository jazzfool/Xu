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

#include <xu/kit/Button.hpp>
#include <xu/core/Theme.hpp>

namespace xu {

Button::Button(Widget* parent, Color inactive, Color hovered, Color clicked) :
    Widget{parent},
    slotHoverEnter{sigOnHoverEnter, this},
    slotHoverExit{sigOnHoverExit, this},
    slotOnClick{sigOnClick, this} {
    currentColor = inactive;
    inactiveColor = inactive;
    hoveredColor = hovered;
    clickedColor = clicked;

    xu::VectorPath vpath
        = xu::VectorPath::RoundRectangle(xu::FSize2{100.0f, 100.0f}, 10.0f);
    paintInfo.paths.push_back(
        vpath.BakeStroke(50.f, 5.f, LineCap::Butt, LineJoin::Bevel));
}

FSize2 Button::SizeHint() const {
    // TODO: measuring text size
    return FSize2{100.f, 100.f};
}

void Button::InitializeTheme(Theme& theme) {
    theme.InitializeWidget(this, &paintInfo, Button::Painter());
}

void Button::Paint(Surface& surf, Theme& theme) const {
    surf.Paint(paintInfo.paths[0], Color::White());
    // theme.PaintWidget(surf, this, &paintInfo, Button::Painter());
}

PainterType Button::Painter() { return typeid(Button); }

void Button::OnHoverEnter() { currentColor = hoveredColor; }
void Button::OnHoverExit() { currentColor = inactiveColor; }
void Button::OnClick(CursorButton) { currentColor = clickedColor; }

} // namespace xu
