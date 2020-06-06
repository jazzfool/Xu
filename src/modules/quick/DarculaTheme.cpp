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

namespace xu::quick {

DarculaTheme::DarculaTheme() {
    params.normalTextSize = 12.f;
    params.headingTextSize = 26.f;
}

Color DarculaTheme::ColorFromPalette(std::string const& colorName) {
    if (colorName == Theme::foregroundTextColor) {
        return Color{230, 230, 230};
    } else if (colorName == Theme::backgroundColor) {
        return Color{50, 50, 50};
    } else if (colorName == Theme::outlineColor) {
        return Color(150, 150, 150);
    } else {
        return Color::Black();
    }
}

void DarculaTheme::PaintWidget(Surface& surf, Widget const* widget,
    PaintInfo const* info, std::type_index basePainter) {
    /*

    if (basePainter == typeid(Button)) {
        Button const* btn = static_cast<Button*>(widget);
        ButtonPaintInfo const* info = static_cast<ButtonPaintInfo*>(info);
        // paint a button
    } else if (basePainter == typeid(TextBox)) {
        Button const* btn = static_cast<Button*>(widget);
        ButtonPaintInfo const* info = static_cast<ButtonPaintInfo*>(info);
        // paint a textbox
    }

    */
}

} // namespace xu::quick
