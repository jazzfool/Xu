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

Button::Button(Widget* parent) : Widget{parent} {}

FSize2 Button::SizeHint() const {
    // TODO: measuring text size
    return FSize2{100.f, 40.f};
}

void Button::GenerateTriangles(
    RenderData& renderData,
    CommandList& cmdList, 
    ISize2 windowSize) const {
    
}

void Button::InitializeTheme(Theme& theme) {
    theme.InitializeWidget(this, &paintInfo, Button::Painter());
}

void Button::Paint(Surface& surf, Theme* theme) const {
    if (theme) theme->PaintWidget(surf, this, &paintInfo, Button::Painter());
}

PainterType Button::Painter() { return typeid(Button); }

} // namespace xu
