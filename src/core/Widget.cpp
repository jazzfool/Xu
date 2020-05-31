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

#include "xu/core/Widget.hpp"

#include "xu/core/WidgetPtr.hpp"

namespace xu {

Widget::Widget(Widget *parent)
    : ownedLayout{nullptr}, parentLayout{nullptr}, parent{parent} {}

Widget::~Widget() {}

void Widget::SetGeometry(FRect2 const &geometry) {
  this->geometry = geometry;

  if (ownedLayout) {
    ownedLayout->Invalidate();
  }

  if (parentLayout) {
    parentLayout->Invalidate();
  }
}

FRect2 Widget::Geometry() const { return geometry; }

Widget *Widget::Parent() const { return parent; }

} // namespace xu