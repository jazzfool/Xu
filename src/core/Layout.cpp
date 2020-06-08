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

static float SizeValue(
    float min, float max, float hint, SizeHintBehaviour behaviour) {
    switch (behaviour) {
        case SizeHintBehaviour::Static: return hint;
        case SizeHintBehaviour::Minimum: return std::max(max, hint);
        case SizeHintBehaviour::Maximum: return std::min(min, hint);
        case SizeHintBehaviour::Preferred:
            return (hint >= min && hint <= max)
                ? hint
                : ((std::abs(max - hint) <= std::abs(min - hint)) ? max : min);
        case SizeHintBehaviour::Expanding:
            return (hint >= min && hint <= max) ? hint : max;
        case SizeHintBehaviour::DontCare: return max;
    }
}

LayoutItem::~LayoutItem() {
    switch (type) {
        case Type::Widget: std::get<0>(item)->layoutItem = nullptr; break;
        case Type::Layout: std::get<1>(item)->layoutItem = nullptr; break;
    }
}

void LayoutItem::SetMaxSize(FSize2 const& max) { maxSize = max; }

FSize2 LayoutItem::MaxSize() const { return maxSize; }

void LayoutItem::SetMinSize(FSize2 const& min) { minSize = min; }

FSize2 LayoutItem::MinSize() const { return minSize; }

FSize2 LayoutItem::PreferredSize() const {
    switch (type) {
        case Type::Widget: return std::get<0>(item)->SizeHint();
        case Type::Layout: return std::get<1>(item)->MinSize();
    }
}

void LayoutItem::SetPosition(FPoint2 const& position) {
    this->position = position;
}

FPoint2 LayoutItem::Position() const { return position; }

bool LayoutItem::Hidden() const {
    switch (type) {
        case Type::Widget: return std::get<0>(item)->hidden;
        case Type::Layout: return false;
    }
}

SizeHintBehaviour LayoutItem::HorizontalSizeHintBehaviour() const {
    switch (type) {
        case Type::Widget:
            return std::get<0>(item)->HorizontalSizeHintBehaviour();
        case Type::Layout:
            return std::get<1>(item)->HorizontalSizeHintBehaviour();
    }
}

SizeHintBehaviour LayoutItem::VerticalSizeHintBehaviour() const {
    switch (type) {
        case Type::Widget:
            return std::get<0>(item)->VerticalSizeHintBehaviour();
        case Type::Layout:
            return std::get<1>(item)->VerticalSizeHintBehaviour();
    }
}

void LayoutItem::Apply() {
    const auto horizHint = HorizontalSizeHintBehaviour();
    const auto vertHint = VerticalSizeHintBehaviour();

    const auto hint = PreferredSize();
    const auto size = FSize2{SizeValue(minSize.x, maxSize.x, hint.x, horizHint),
        SizeValue(minSize.y, maxSize.y, hint.y, vertHint)};

    switch (type) {
        case Type::Widget:
            std::get<0>(item)->geometry.size = size;
            std::get<0>(item)->geometry.origin = position;
            break;
        case Type::Layout:
            std::get<1>(item)->geometry.size = size;
            std::get<1>(item)->geometry.origin = position;
            break;
    }
}

LayoutItem::LayoutItem(Widget* widget) : type{Type::Widget}, item{widget} {
    widget->layoutItem = this;
}

LayoutItem::LayoutItem(std::unique_ptr<Layout> layout) :
    type{Type::Layout},
    item{std::move(layout)} {
    layout->layoutItem = this;
}

Layout::Layout() :
    geometry{{0.f, 0.f}, {0.f, 0.f}},
    parentLayout{nullptr},
    layoutItem{nullptr},
    horizontalShb{SizeHintBehaviour::Preferred},
    verticalShb{SizeHintBehaviour::Preferred},
    invalid{true} {}

Layout::~Layout() {
    for (auto const& item : items) {
        switch (item.index()) {
            case 0: std::get<0>(item)->parentLayout = nullptr; break;
            case 1: std::get<1>(item)->parentLayout = nullptr; break;
        }
    }
}

void Layout::Update(FRect2 const& rect) {
    if (invalid) {
        UpdateItems();
        invalid = false;
    }

    for (auto const& item : items) {
        if (item.index() == 1) { std::get<1>(item)->Update(); }
    }
}

void Layout::Invalidate() {
    invalid = true;
    for (auto const& item : items) {
        if (item.index() == 1) { std::get<1>(item)->Invalidate(); }
    }
}

void Layout::InsertWidget(std::size_t where, Widget* widget) {
    XU_ASSERT(widget->parentLayout == nullptr);
    XU_ASSERT(widget->ownedLayout.get() != this);

    widget->parentLayout = this;

    items.push_back(widget);
    InsertItem(where, LayoutItem{widget});
}

void Layout::AddWidget(Widget* widget) { InsertWidget(NumItems(), widget); }

void Layout::InsertLayout(std::size_t where, std::unique_ptr<Layout> layout) {
    XU_ASSERT(layout->parentLayout == nullptr);

    layout->parentLayout = this;

    items.push_back(layout.get());
    InsertItem(where, LayoutItem{std::move(layout)});
}

void Layout::AddLayout(std::unique_ptr<Layout> layout) {
    InsertLayout(NumItems(), std::move(layout));
}

void Layout::SetGeometry(FRect2 const& geometry) {
    this->geometry = geometry;
    Invalidate();
    OnGeometryChanged();
}

FRect2 Layout::Geometry() const { return geometry; }

void Layout::SetHorizontalSizeHintBehaviour(SizeHintBehaviour shb) {
    horizontalShb = shb;
    if (layoutItem) { layoutItem->Apply(); }
}

SizeHintBehaviour Layout::HorizontalSizeHintBehaviour() const {
    return horizontalShb;
}

void Layout::SetVerticalSizeHintBehaviour(SizeHintBehaviour shb) {
    verticalShb = shb;
    if (layoutItem) { layoutItem->Apply(); }
}

SizeHintBehaviour Layout::VerticalSizeHintBehaviour() const {
    return verticalShb;
}

} // namespace xu
