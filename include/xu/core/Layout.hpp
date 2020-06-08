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

#include "Definitions.hpp"
#include "Rect2.hpp"
#include "Size2.hpp"

#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

namespace xu {

class Widget;
class Layout;

/*!
 * \brief Modes of how Widget::SizeHint should be treated by LayoutItem.
 */
enum class SizeHintBehaviour {
    /*!
     * \brief The Widget::SizeHint value is the final size always; no more, no
     * less.
     */
    Static,
    /*!
     * \brief The Widget::SizeHint value is the minimum size, it cannot be any
     * less, but it can be more.
     */
    Minimum,
    /*!
     * \brief The Widget::SizeHint value is the maxmimum size, it cannot be any
     * more, but it can be less.
     */
    Maximum,
    /*!
     * \brief The Widget::SizeHint value is the preferred size and should be
     * applied where possible, but it may be either less or more.
     */
    Preferred,
    /*!
     * \brief The Widget::SizeHint value is the sensible (default) size,
     * but it is preferred that as much space as possible be taken up.
     */
    Expanding,
    /*!
     * \brief The Widget::SizeHint value should be completely ignored. Whatever
     * size is given by layout (i.e. the maximum) is what should be applied.
     */
    DontCare
};

/*!
 * \brief Mediating class which acts as a proxy for anything that can be placed
 * in a layout (namely other layouts and widgets).
 * \sa Layout
 */
class XU_API LayoutItem final {
public:
    LayoutItem() = default;
    LayoutItem(LayoutItem const&) = delete;
    LayoutItem(LayoutItem&&) = default;
    LayoutItem& operator=(LayoutItem const&) = delete;
    LayoutItem& operator=(LayoutItem&&) = default;

    ~LayoutItem();

    /*!
     * \brief Changes the maximum geometry size available to the underlying
     * object.
     * \sa LayoutItem::MaxSize
     */
    void SetMaxSize(FSize2 const& max);
    /*!
     * \brief Returns the maximum geometry size available to the underlying
     * object.
     * \sa LayoutItem::SetMaxSize
     */
    FSize2 MaxSize() const;
    /*!
     * \brief Changes the minimum geometry size available to the underlying
     * object.
     * \sa LayoutItem::MinSize
     */
    void SetMinSize(FSize2 const& min);
    /*!
     * \brief Returns the minimum geometry size available to the underlying
     * object.
     * \sa LayoutItem::SetMinSize
     */
    FSize2 MinSize() const;
    /*!
     * \brief Returns the preferred size of the underlying object. For widgets
     * this is Widget::SizeHint, and for layouts this is Layout::MinSize.
     * \sa Widget::SizeHint, Layout::MinSize
     */
    FSize2 PreferredSize() const;

    /*!
     * \brief Changes the geometry position of the underlying
     * object.
     * \sa LayoutItem::Position
     */
    void SetPosition(FPoint2 const& position);
    /*!
     * \brief Returns the geometry position of the underlying
     * object.
     * \sa LayoutItem::SetPosition
     */
    FPoint2 Position() const;

    /*!
     * \brief Returns the hidden state of this object. That is to say; should
     * its rectangle be considered in the layout.
     * \sa Widget::hidden
     */
    bool Hidden() const;

    /*!
     * \brief Returns how the underlying item's preferred width should be
     * treated.
     */
    SizeHintBehaviour HorizontalSizeHintBehaviour() const;
    /*!
     * \brief Returns how the underlying item's preferred height should be
     * treated.
     */
    SizeHintBehaviour VerticalSizeHintBehaviour() const;

    /*!
     * \brief Updates the actual geometry using the item's SizeHintBehaviour,
     * MaxSize and MinSize
     */
    void Apply();

private:
    friend class Layout;

    LayoutItem(Widget* widget);
    LayoutItem(std::unique_ptr<Layout> layout);

    FPoint2 position;
    FSize2 maxSize;
    FSize2 minSize;

    enum class Type { Widget, Layout } type;
    std::variant<Widget*, std::unique_ptr<Layout>> item;
};

/*!
 * \brief Minimal layout interface which is capable of selecting a minimum
 * fitting size and changing the geometry of its items when invalidated. The
 * implementor of this interface is responsible for storing children
 * via Layout::InsertItem and Layout::NumItems.
 * \sa LayoutItem, Widget
 */
class XU_API Layout {
public:
    Layout();
    virtual ~Layout();

    /*!
     * \brief Computes the minimum size required by this layout to fit all its
     * item. Normally this is a rectangular convex hull.
     * \sa Widget::SizeHint
     */
    virtual FSize2 MinSize() const = 0;
    /*!
     * \brief Number of items present in this layout.
     */
    virtual std::size_t NumItems() const = 0;

    /*!
     * \brief Updates the geometry for each item in whatever manner deemed
     * suitable by the implementation. The underlying implementation method is
     * only called if the layout has been invalidated (the invalidation process
     * is handled automatically). This will propagate to all child layouts.
     * \sa LayoutItem::SetRect
     */
    virtual void Update(FRect2 const& rect) final;
    /*!
     * \brief Forcefully invalidates this layout. Note: This does not call
     * Layout::Update.
     */
    virtual void Invalidate() final;

    /*!
     * \brief Insert a widget item at a certain index.
     * \sa AddWidget, InsertLayout
     */
    virtual void InsertWidget(std::size_t where, Widget* widget) final;
    /*!
     * \brief Adds a widget item onto the end.
     * \sa InsertWidget, AddLayout
     */
    virtual void AddWidget(Widget* widget) final;

    /*!
     * \brief Insert a layout item at a certain index.
     * \sa AddLayout, InsertWidget
     */
    virtual void InsertLayout(
        std::size_t where, std::unique_ptr<Layout> layout) final;
    /*!
     * \brief Adds a layout item onto the end.
     * \sa InsertLayout, AddWidget
     */
    virtual void AddLayout(std::unique_ptr<Layout> layout) final;

    /*!
     * \brief Changes the layout geometry. This is the same geometry that is
     * typically used by the implementor to infer the size available for item
     * geometry layout.
     * \sa Geometry
     */
    virtual void SetGeometry(FRect2 const& geometry) final;

    /*!
     * \brief Returns the layout geometry.
     * \sa SetGeometry
     */
    virtual FRect2 Geometry() const final;

    /*!
     * \brief Changes the horizontal (width) SizeHint behaviour.
     */
    virtual void SetHorizontalSizeHintBehaviour(SizeHintBehaviour shb) final;
    /*!
     * \brief Returns the horizontal (width) SizeHint behaviour.
     */
    virtual SizeHintBehaviour HorizontalSizeHintBehaviour() const final;
    /*!
     * \brief Changes the vertical (height) SizeHint behaviour.
     */
    virtual void SetVerticalSizeHintBehaviour(SizeHintBehaviour shb) final;
    /*!
     * \brief Returns the vertical (height) SizeHint behaviour.
     */
    virtual SizeHintBehaviour VerticalSizeHintBehaviour() const final;

protected:
    /*!
     * \brief Request for an item insertion at a certain index.
     */
    virtual void InsertItem(std::size_t where, LayoutItem item) = 0;
    /*!
     * \brief Updates all the stored LayoutItem geometry.
     * \sa LayoutItem::SetRect
     */
    virtual void UpdateItems() = 0;
    /*!
     * \brief Invoked whenever SetGeometry is invoked.
     */
    virtual void OnGeometryChanged() {}

private:
    friend class LayoutItem;

    FRect2 geometry;
    Layout* parentLayout;
    LayoutItem* layoutItem;

    SizeHintBehaviour horizontalShb;
    SizeHintBehaviour verticalShb;

    bool invalid;
    std::vector<std::variant<Widget*, Layout*>>
        items; //!< The implementing class should have its own list of
               //!< LayoutItems, but we also maintain an (orderless) list for
               //!< various purposes.
};

} // namespace xu
