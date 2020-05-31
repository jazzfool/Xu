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

#include "nano_signal_slot.hpp"

namespace xu {

// RAII-like slot object that automatically disconnects the slot from the signal
// when destructed.
template <auto X> class UniqueSlot final {};

template <typename T, typename... Ts, void (T::*F)(Ts...)> class UniqueSlot<F> {
public:
  UniqueSlot(Nano::Signal<void(Ts...)> &sig, T *obj) : signal{&sig}, obj{obj} {
    signal->template connect<F>(obj);
  }

  ~UniqueSlot() { signal->template disconnect<F>(obj); }

  Nano::Signal<void(Ts...)> *signal;

private:
  T *obj;
};

template <typename... Ts, void (*F)(Ts...)> class UniqueSlot<F> {
public:
  UniqueSlot(Nano::Signal<void(Ts...)> &sig) : signal{&sig} {
    signal->template connect<F>();
  }

  ~UniqueSlot() { signal->template disconnect<F>(); }

  Nano::Signal<void(Ts...)> *signal;
};

} // namespace xu
