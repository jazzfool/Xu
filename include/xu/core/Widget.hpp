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

#include <xu/core/Definitions.hpp>
#include <xu/core/Layout.hpp>
#include <xu/core/Point2.hpp>
#include <xu/core/RenderData.hpp>
#include <xu/core/Size2.hpp>
#include <xu/core/Surface.hpp>
#include <xu/core/UniqueSlot.hpp>
#include <xu/core/WidgetPtr.hpp>

#include <memory>
#include <vector>

namespace xu {

/*!
 * \brief Core widget class of the library. All widgets must derive from this
 * class.
 */
class XU_API Widget {
public:
    /*!
     * \brief Widget constructor
     * \param parent Pointer to the parent of this widget. May be nullptr.
     */
    explicit Widget(Widget* parent);
    virtual ~Widget();

    /*!
     * \brief Gives the layout system a hint of how large this widget has to be.
     *        Note that the actual size may be different depending on the
     * layout. \sa [Insert layout documentation page]
     */
    virtual FSize2 SizeHint() const = 0;

    /*!
     * \brief Paint this widget's visual representation onto the surface. This
     * function must be overridden by child widgets.
     */
    virtual void Paint(Surface& surface) const = 0;

    /*!
     * \brief Output this widget's visual representation by pushing triangle
     * draw commands into the command list. Note: This API will be removed once
     * vector path graphics are implemented.
     */
    virtual void GenerateTriangles(
        RenderData& renderData,
        CommandList& cmdList,
        ISize2 windowSize) const = 0;

    /*!
     * \brief Test whether the mouse pointer is hovering the widget. Custom
     * widgets may override this to replace the default AABB intersection test.
     * \param pointer Point with mouse coordinates. TODO: Document coordinate
     * system
     */
    virtual bool PointerHit(FPoint2 const& pointer) const;

    /*!
     * \brief Set geometry to be used for building the layout.
     *\param geometry AABB defining the bounding box of this widget.
     */
    virtual void SetGeometry(FRect2 const& geometry) final;

    /*!
     * \brief Access the geometry AABB of this widget.
     */
    virtual FRect2 Geometry() const final;

    /*!
     * \brief Obtain a pointer to the parent of this widget. If there is no
     * parent, nullptr is returned.
     */
    virtual Widget* Parent() const final;

    /*!
     * \brief Obtain the amount of children this widget owns. Useful for
     * iterating over them.
     */
    virtual std::size_t NumChildren() const final;

    /*!
     * \brief Create a new child widget of type T and append it to the list of
     * children. \param args Arguments to be forwarded to the widget's
     * constructor. \return WidgetPtr to the child widget. \sa WidgetPtr
     */
    template<typename T, typename... CtorArgs>
    WidgetPtr<T> MakeChild(CtorArgs&&... args) {
        auto child = std::make_unique<T>(this, std::forward<CtorArgs>(args)...);
        children.push_back(std::move(child));
        return WidgetPtr<T>(static_cast<T*>(children.back().get()));
    }

    /*!
     * \brief Create a new child widget of type T and put it at the specified
     * index in the list of children. \param at Position to insert the new child
     * widget. \param args Arguments to be forwarded to the widget's constructor
     * \return WidgetPtr to the child widget.
     * \sa WidgetPtr
     */
    template<typename T, typename... CtorArgs>
    WidgetPtr<T> MakeChildAt(std::size_t at, CtorArgs&&... args) {
        auto child = std::make_unique<T>(this, std::forward<CtorArgs>(args)...);
        children.insert(at, std::move(child));
        return WidgetPtr<T>(static_cast<T*>(children.back().get()));
    }

    /*!
     * \brief Obtain a pointer to the widget at index at
     * \param at Index of the child widget to get a pointer to.
     */
    virtual Widget* GetChild(std::size_t at) final;

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

    /*!
     * \brief Signal to be called before destroying this widget
     */
    Signal<> sigBeforeDestruction;

    /*!
     * \brief Toggle to indicate whether this widget is hidden or not. This
     * propagates to child widgets.
     */
    bool hidden;

private:
    friend class LayoutItem;
    friend class Layout;

    FRect2 geometry;
    std::unique_ptr<Layout>
        ownedLayout;      //!< Layout this widget owns (possibly nullptr).
    Layout* parentLayout; //!< Layout this widget is in (possibly nullptr).
    LayoutItem* layoutItem;

    SizeHintBehaviour horizontalShb;
    SizeHintBehaviour verticalShb;

    Widget* parent;
    std::vector<std::unique_ptr<Widget>> children;
};

} // namespace xu