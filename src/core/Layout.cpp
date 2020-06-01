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

#include <xu/core/Layout.hpp>
#include <xu/core/Widget.hpp>

namespace xu {

void LayoutItem::SetRect(FRect2 const& rect) {
    switch (type) {
        case Type::Widget: std::get<0>(item)->SetGeometry(rect); break;
        case Type::Layout: std::get<1>(item)->SetGeometry(rect); break;
    }
}

Layout::~Layout() {}

void Layout::Update(FRect2 const& rect) {
    if (invalid) {
        UpdateItems();
        invalid = false;
    }
}

void Layout::Invalidate() {
    invalid = true;
    for (auto const& layout : childLayouts) { layout->Invalidate(); }
}

void Layout::InsertWidget(std::size_t where, Widget* widget) {
    InsertItem(where, LayoutItem{widget});
}

void Layout::AddWidget(Widget* widget) {
    InsertItem(NumItems(), LayoutItem{widget});
}

void Layout::InsertLayout(std::size_t where, std::unique_ptr<Layout> layout) {
    InsertItem(where, LayoutItem{std::move(layout)});
    childLayouts.push_back(layout.get());
}

void Layout::AddLayout(std::unique_ptr<Layout> layout) {
    InsertItem(NumItems(), LayoutItem{std::move(layout)});
    childLayouts.push_back(layout.get());
}

void Layout::SetGeometry(FRect2 const& geometry) {
    this->geometry = geometry;
    Invalidate();
    OnGeometryChanged();
}

FRect2 Layout::Geometry() const { return geometry; }

} // namespace xu
