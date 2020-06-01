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

#include <xu/core/UniqueSlot.hpp>

namespace xu {

class Widget;

/*!
 * \brief Special immutable widget pointer wrapper which can nullify the pointer
 * before the widget is destroyed.
 */
template<typename T>
struct WidgetPtr {
    WidgetPtr(T* ptr) :
        ptr{ptr},
        slotBeforeDestruction{ptr->sigBeforeDestruction, this} {}

    WidgetPtr(const WidgetPtr& other) :
        ptr{other.ptr},
        slotBeforeDestruction{other.ptr->sigBeforeDestruction, this} {}

    WidgetPtr(WidgetPtr&& other) :
        ptr{other.ptr},
        slotBeforeDestruction{std::move(other.slotBeforeDestruction)} {
        other.ptr = nullptr;
    }

    WidgetPtr& operator=(const WidgetPtr& other) {
        if (this != &other) {
            ptr = other.ptr;
            slotBeforeDestruction = UniqueSlot{ptr->sigBeforeDestruction, this};
        }
        return *this;
    }

    WidgetPtr& operator=(WidgetPtr&& other) {
        if (this != &other) {
            ptr = other.ptr;
            slotBeforeDestruction = std::move(other.slotBeforeDestruction);
            other.ptr = nullptr;
        }
        return *this;
    }

    Widget* get() const { return static_cast<Widget*>(ptr); }

    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
    operator bool() const { return ptr != nullptr; }

private:
    void BeforeDestruction() { ptr = nullptr; }

    T* ptr;
    const UniqueSlot<&WidgetPtr::BeforeDestruction> slotBeforeDestruction;
};

} // namespace xu