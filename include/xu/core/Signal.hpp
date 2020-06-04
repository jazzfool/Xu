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

/*!
 * \brief Signal/slot type which emits events with variadic template arguments
 * (hence the variadic templates). Slots can be connected with member function
 * pointers or regular function pointers.
 * \sa UniqueSlot
 */
template<typename... Ts>
class Signal final {
public:
    ~Signal() {
        for (auto const& slot : uniqueSlots) {
            if (slot) *slot = nullptr;
        }
    }

    /*!
     * \brief Connects a member function pointer callback (the first template
     * parameter) with a pointer to an instance of the class (the first and only
     * parameter).
     */
    template<auto MemFnPtr, typename T>
    void Connect(T* instance) {
        sig.template connect<MemFnPtr, T>(instance);
    }

    /*!
     * \brief Connects a member function pointer callback (the first template
     * parameter) with a reference to an instance of the class (the first and
     * only parameter).
     */
    template<auto MemFnPtr, typename T>
    void Connect(T& instance) {
        sig.template connect<MemFnPtr, T>(instance);
    }

    /*!
     * \brief Connects a function pointer callback (the first template
     * parameter).
     */
    template<auto FnPtr>
    void Connect() {
        sig.template connect<FnPtr>();
    }

    /*!
     * \brief Disconnects a previously connected callback. This is an exact
     * reverse operation of Connect and thus has the same parameters.
     */
    template<auto MemFnPtr, typename T>
    void Disconnect(T* instance) {
        sig.template disconnect<MemFnPtr, T>(instance);
    }

    /*!
     * \brief Disconnects a previously connected callback. This is an exact
     * reverse operation of Connect and thus has the same parameters.
     */
    template<auto MemFnPtr, typename T>
    void Disconnect(T& instance) {
        sig.template disconnect<MemFnPtr, T>(instance);
    }

    /*!
     * \brief Disconnects a previously connected callback. This is an exact
     * reverse operation of Connect and thus has the same parameters.
     */
    template<auto FnPtr>
    void Disconnect() {
        sig.template disconnect<FnPtr>();
    }

    /*!
     * \brief Emits an event with specified arguments, essentially invoking all
     * the connected callbacks.
     */
    inline void Emit(Ts&&... arg) { sig.fire(std::forward<Ts>(arg)...); }
    /*!
     * \brief Same function as Emit.
     * \sa Emit
     */
    inline void operator()(Ts&&... arg) { sig.fire(std::forward<Ts>(arg)...); }

private:
    using SignalT = Nano::Signal<void(Ts...)>;
    template<auto X>
    friend class UniqueSlot;

    std::uint32_t AddSlot(Signal*& slot) {
        uniqueSlots.push_back(&slot);
        return uniqueSlots.size() - 1;
    }

    inline void SetSlot(std::uint32_t slot, Signal*& value) {
        uniqueSlots[slot] = &value;
    }

    SignalT sig;
    std::vector<Signal**> uniqueSlots;
};

} // namespace xu
