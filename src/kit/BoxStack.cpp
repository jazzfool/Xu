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
    const auto os = OrientationSubject();
    std::size_t numVisible = 0; // num of *visible* (non-hidden) children
    FSize2 sz{0.f, 0.f};
    for (auto const& item : items) {
        if (item.Hidden()) { continue; }
        sz.*os += item.PreferredSize().*os;
        numVisible += 1;
    }

    if (numVisible == 0) { return FSize2{0.f, 0.f}; }
    sz.*os += spacing * (numVisible - 1);

    return sz;
}

std::size_t BoxStack::NumItems() const { return items.size(); }

void BoxStack::InsertItem(std::size_t where, LayoutItem item) {
    items.insert(items.begin() + where, std::move(item));
}

void BoxStack::UpdateItems() {
    if (NumItems() == 0) return;

    const auto os = OrientationSubject();

    auto itemSize = Geometry().size;
    itemSize.*os = (Geometry().size.*os / NumItems()) - spacing;

    auto pos = Geometry().origin;
    for (auto& item : items) {
        if (item.Hidden()) { continue; }

        item.SetPosition(pos);
        item.SetMinSize({0.f, 0.f});
        item.SetMaxSize(itemSize);

        item.Apply();

        pos.*os += itemSize.*os + spacing;
    }
}

float FSize2::*BoxStack::OrientationSubject() const {
    switch (stackOrientation) {
        case StackOrientation::Vertical: return &FSize2::y;
        case StackOrientation::Horizontal: return &FSize2::x;
    }
}

} // namespace xu
