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
#include <vector>

namespace xu {

template <typename... Ts> class Signal;
namespace detail {
template <typename... Ts> struct SlotForSignal { Signal<Ts...> *signal; };
} // namespace detail

template <typename... Ts> class Signal final {
public:
  using SignalT = Nano::Signal<void(Ts...)>;

  Signal() : nextSlotKey{0} {}

  ~Signal() {
    for (auto const &slot : uniqueSlots) {
      if (slot)
        slot->signal = nullptr;
    }
  }

  SignalT *operator->() { return &sig; }
  SignalT &operator*() { return sig; }
  const SignalT *operator->() const { return &sig; }
  const SignalT &operator*() const { return sig; }

private:
  template <auto X> friend class UniqueSlot;

  std::uint32_t AddSlot(detail::SlotForSignal<Ts...> *slot) {
    uniqueSlots.push_back(slot);
    return uniqueSlots.size() - 1;
  }

  inline void SetSlot(std::uint32_t slot, detail::SlotForSignal<Ts...> *value) {
    uniqueSlots[slot] = value;
  }

  SignalT sig;
  std::uint32_t nextSlotKey;
  std::vector<detail::SlotForSignal<Ts...> *> uniqueSlots;
};

// RAII-like slot object that automatically disconnects the slot from the signal
// when destructed.
template <auto X> class UniqueSlot final {};

template <typename T, typename... Ts, void (T::*F)(Ts...)>
class UniqueSlot<F> : private detail::SlotForSignal<Ts...> {
public:
  UniqueSlot(Signal<Ts...> &sig, T *obj)
      : signal{&sig}, obj{obj}, slotKey{sig.AddSlot(this)} {
    signal->template connect<F>(obj);
  }

  UniqueSlot(UniqueSlot &&other)
      : signal{other.signal}, obj{other.obj}, slotKey{other.slotKey} {
    other.signal = nullptr;
    other.obj = nullptr;
    signal->SetSlot(slotKey, this);
  }

  UniqueSlot(const UniqueSlot &) = delete;

  ~UniqueSlot() {
    if (signal)
      signal->template disconnect<F>(obj);
  }

  UniqueSlot &operator=(const UniqueSlot &) = delete;

private:
  T *obj;
  std::uint32_t slotKey;
};

template <typename... Ts, void (*F)(Ts...)>
class UniqueSlot<F> : private detail::SlotForSignal<Ts...> {
public:
  UniqueSlot(Nano::Signal<void(Ts...)> &sig) : signal{&sig} {
    signal->template connect<F>();
  }

  UniqueSlot(UniqueSlot &&other) : signal{other.signal} {
    other.signal = nullptr;
  }

  UniqueSlot(const UniqueSlot &) = delete;

  ~UniqueSlot() {
    if (signal)
      signal->template disconnect<F>();
  }

  UniqueSlot &operator=(const UniqueSlot &) = delete;

private:
  std::uint32_t slotKey;
};

} // namespace xu
