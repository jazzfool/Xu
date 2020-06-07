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
#include <xu/core/Size2.hpp>
#include <xu/core/Point2.hpp>
#include <xu/core/Vector2.hpp>
#include <xu/core/InputEnums.hpp>

namespace xu {

/*!
 * \brief Struct used to describe a window resize event.
 * \sa [Insert link to eventing docs]
 */
struct XU_API WindowResizeEvent {
    /*!
     * \brief ID of the window being resized.
     */
    WindowID id;

    /*!
     * \brief New window size.
     */
    ISize2 size;
};

/*!
 * \brief Struct used to describe a window move event.
 */
struct XU_API WindowMoveEvent {
    /*!
     * \brief ID of the window being resized.
     */
    WindowID id;

    /*!
     * \brief New window position.
     */
    IPoint2 position;
};

struct XU_API WindowCursorEnterEvent {
    /*!
     * \brief ID of the window the cursor entered/left.
     */
    WindowID id;

    /*!
     * \brief Cursor entered if set to true
     *        Cursor left if set to false
     */
    bool entered;
};

struct XU_API CursorMoveEvent {
    /*!
     * \brief New mouse position in global pixel coordinates.
     *        Relative to the top left of the primary display.
     */
    IPoint2 position;

    /*!
     * \brief Mouse movement since last tick.
     *        Measured in pixel coordinates.
     */
    IVector2 positionDelta;
};

struct XU_API CursorButtonEvent {
    /*!
     * \brief Cursor button that was had an event.
     */
    CursorButton button;
    /*!
     * \brief Set to true if the button was clicked. If not, set to false.
     */
    bool value;
};

} // namespace xu