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

#include <xu/core/Signal.hpp>

namespace xu {

// RAII-like slot object that automatically disconnects the slot from the signal
// when destructed.
template<auto X>
class UniqueSlot final {};

template<typename T, typename... Ts, void (T::*F)(Ts...)>
class UniqueSlot<F> {
public:
    UniqueSlot(Signal<Ts...>& sig, T* obj) :
        signal{&sig},
        obj{obj},
        slotKey{sig.AddSlot(signal)} {
        signal->template Connect<F>(obj);
    }

    UniqueSlot(UniqueSlot&& other) :
        signal{other.signal},
        obj{other.obj},
        slotKey{other.slotKey} {
        other.signal = nullptr;
        other.obj = nullptr;
        signal->SetSlot(slotKey, this);
    }

    UniqueSlot(const UniqueSlot&) = delete;

    ~UniqueSlot() {
        if (signal) signal->template Disconnect<F>(obj);
    }

    UniqueSlot& operator=(const UniqueSlot&) = delete;

    UniqueSlot& operator=(UniqueSlot&& other) {
        if (this != &other) {
            signal = other.signal;
            obj = other.obj;
            slotKey = other.slotKey;

            other.signal = nullptr;
            other.obj = nullptr;
            signal->SetSlot(slotKey, this);
        }
        return *this;
    }

private:
    Signal<Ts...>* signal;
    T* obj;
    std::uint32_t slotKey;
};

template<typename... Ts, void (*F)(Ts...)>
class UniqueSlot<F> {
public:
    UniqueSlot(Signal<Ts...>& sig) :
        signal{&sig},
        slotKey{sig.AddSlot(signal)} {
        signal->template Connect<F>();
    }

    UniqueSlot(UniqueSlot&& other) : signal{other.signal} {
        other.signal = nullptr;
    }

    UniqueSlot(const UniqueSlot&) = delete;

    ~UniqueSlot() {
        if (signal) signal->template Disconnect<F>();
    }

    UniqueSlot& operator=(const UniqueSlot&) = delete;

    UniqueSlot& operator=(UniqueSlot&& other) {
        if (this != &other) {
            signal = other.signal;
            slotKey = other.slotKey;

            other.signal = nullptr;
            signal->SetSlot(slotKey, this);
        }
        return *this;
    }

private:
    Signal<Ts...>* signal;
    std::uint32_t slotKey;
};

} // namespace xu