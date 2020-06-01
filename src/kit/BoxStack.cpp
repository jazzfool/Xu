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

#include <xu/kit/BoxStack.hpp>

namespace xu {

BoxStack::BoxStack() :
    stackOrientation{StackOrientation::Vertical},
    spacing{0.f} {}

FSize2 BoxStack::MinSize() const {
    if (NumItems() == 0) { return FSize2{0.f, 0.f}; }

    const auto os = OrientationSubject();
    FSize2 sz{0.f, 0.f};
    for (auto const& item : items) { sz.*os += item.Rect().size.*os; }
    sz.*os += spacing * (NumItems() - 1);

    return sz;
}

std::size_t BoxStack::NumItems() const { return items.size(); }

void BoxStack::InsertItem(std::size_t where, LayoutItem item) {
    items.insert(items.begin() + where, std::move(item));
}

void BoxStack::UpdateItems() {
    const auto os = OrientationSubject();
    auto pos = Geometry().origin;
    for (auto& item : items) {
        item.SetRect(FRect2{pos, item.Rect().size});
        pos.*os += item.Rect().size.*os + spacing;
    }
}

float FSize2::*BoxStack::OrientationSubject() const {
    switch (stackOrientation) {
        case StackOrientation::Vertical: return &FSize2::y;
        case StackOrientation::Horizontal: return &FSize2::x;
    }
}

} // namespace xu
