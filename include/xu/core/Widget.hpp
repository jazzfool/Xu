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
#include <xu/core/Size2.hpp>
#include <xu/core/UniqueSlot.hpp>
#include <xu/core/WidgetPtr.hpp>

#include <memory>
#include <vector>

namespace xu {

class XU_API Widget {
public:
  explicit Widget(Widget *parent);
  virtual ~Widget();

  virtual FSize2 SizeHint() const = 0;

  virtual void SetGeometry(FRect2 const &geometry) final;
  virtual FRect2 Geometry() const final;

  virtual Widget *Parent() const final;

  template <typename T, typename... CtorArgs>
  WidgetPtr<T> MakeChild(CtorArgs &&... arg) {
    auto child = std::make_unique<T>(this, std::forward<CtorArgs>(arg)...);
    children.push_back(std::move(child));
    return WidgetPtr{child.back().get()};
  }

  Signal<> sigBeforeDestruction;

private:
  FRect2 geometry;
  std::unique_ptr<Layout>
      ownedLayout;      //!< Layout this widget owns (possibly nullptr).
  Layout *parentLayout; //!< Layout this widget is in (possibly nullptr).

  Widget *parent;
  std::vector<std::unique_ptr<Widget>> children;
};

} // namespace xu