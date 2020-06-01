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

struct XU_API LayoutItem final {
public:
  void SetRect(FRect2 const &rect);
  FRect2 Rect() const;

private:
  friend class Layout;
  LayoutItem(Widget *widget) : type{Type::Widget}, item{widget} {}
  LayoutItem(std::unique_ptr<Layout> layout)
      : type{Type::Layout}, item{std::move(layout)} {}

  enum class Type { Widget, Layout } type;
  std::variant<Widget *, std::unique_ptr<Layout>> item;
};

class XU_API Layout {
public:
  virtual ~Layout();

  virtual FSize2 MinSize() const = 0;
  virtual std::size_t NumItems() const = 0;

  virtual void Update(FRect2 const &rect) final;
  virtual void Invalidate() final;

  virtual void InsertWidget(std::size_t where, Widget *widget) final;
  virtual void AddWidget(Widget *widget) final;

  virtual void InsertLayout(std::size_t where,
                            std::unique_ptr<Layout> layout) final;
  virtual void AddLayout(std::unique_ptr<Layout> layout) final;

  virtual void SetGeometry(FRect2 const &geometry) final;
  virtual FRect2 Geometry() const final;

protected:
  virtual void InsertItem(std::size_t where, LayoutItem item) = 0;
  virtual void UpdateItems() = 0;
  virtual void OnGeometryChanged() {}

private:
  FRect2 geometry;
  bool invalid;
  std::vector<Layout *>
      childLayouts; //!< We need to keep a list of child layouts so that we can
                    //!< propagate layout invalidation.
};

} // namespace xu
