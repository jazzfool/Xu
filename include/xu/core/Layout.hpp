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

    /*!
     * \brief Changes the geometry rectangle of the underlying object.
     * \sa LayoutItem::Rect
     */
    void SetRect(FRect2 const& rect);
    /*!
     * \brief Returns the geometry rectangle of the underlying object.
     * \sa LayoutItem::SetRect
     */
    FRect2 Rect() const;
    /*!
     * \brief Returns the preferred size of the underlying object. For widgets
     * this is Widget::SizeHint, and for layouts this is Layout::MinSize.
     * \sa Widget::SizeHint, Layout::MinSize
     */
    FSize2 PreferredSize() const;

    /*!
     * \brief Returns the hidden state of this object. That is to say; should
     * its rectangle be considered in the layout.
     * \sa Widget::hidden
     */
    bool Hidden() const;

private:
    friend class Layout;
    LayoutItem(Widget* widget) : type{Type::Widget}, item{widget} {}
    LayoutItem(std::unique_ptr<Layout> layout) :
        type{Type::Layout},
        item{std::move(layout)} {}

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
     * is handled automatically).
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
    FRect2 geometry;
    bool invalid;
    std::vector<Layout*>
        childLayouts; //!< We need to keep a list of child layouts so that we
                      //!< can propagate layout invalidation.
};

} // namespace xu
