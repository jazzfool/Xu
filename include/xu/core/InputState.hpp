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
#include <xu/core/Vector2.hpp>
#include <xu/core/InputEnums.hpp>

namespace xu {

class XU_API InputState {
public:
    IVector2 cursorPosition;
    IVector2 cursorPositionDelta;

    [[nodiscard]] bool GetCursorButton(CursorButton btn) const {
        return cursorButtonStates[(int)btn];
    }

    void SetCursorButton(CursorButton btn, bool newState) {
        cursorButtonStates[(int)btn] = newState;
    }

    void ToggleCursorButton(CursorButton btn) {
        cursorButtonStates[(int)btn] = !cursorButtonStates[(int)btn];
    }

private:
    bool cursorButtonStates[(int)CursorButton::COUNT] = {};
};

} // namespace xu