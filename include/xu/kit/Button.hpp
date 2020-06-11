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

#include <xu/core/Color.hpp>
#include <xu/core/Widget.hpp>
#include <xu/core/Theme.hpp>

namespace xu {

class Button : public Widget {
public:
    // Until theme is used, color parameters
    Button(Widget* parent, Color inactive, Color hovered, Color clicked);

    FSize2 SizeHint() const override;

    void InitializeTheme(Theme& theme) override;
    void Paint(Surface& surf, Theme* theme) const override;

    static PainterType Painter();

protected:
    void OnHoverEnter();
    void OnHoverExit();
    void OnClick(CursorButton button);

    UniqueSlot<&Button::OnHoverEnter> slotHoverEnter;
    UniqueSlot<&Button::OnHoverExit> slotHoverExit;
    UniqueSlot<&Button::OnClick> slotOnClick;

    PaintInfo paintInfo;
    Color currentColor;
    Color inactiveColor;
    Color hoveredColor;
    Color clickedColor;
};

} // namespace xu
