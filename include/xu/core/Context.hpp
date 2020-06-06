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
#include <xu/core/Events.hpp>
#include <xu/core/RenderData.hpp>
#include <xu/core/Surface.hpp>
#include <xu/core/WsiInterface.hpp>

// Temporary?
#include <xu/core/Widget.hpp>

#include <queue>

namespace xu {

/*!
 * \brief Select which method must be used for event processing.
 */
enum class InputReception {
    /*!
     * \brief Indicates that events will be dispatched in order of submission
     * (FIFO) on calling Context::ProcessEvents()
     */
    Queued,
    /*!
     * \brief Indicates that events will be dispatched immediately as they are
     * received.
     */
    Immediate
};

/*!
 * \brief Core context class of the Xu library.
 */
class XU_API Context {
public:
    /*!
     * \brief Notifies Xu that a mouse move event has occured.
     * \sa [Insert windowing event docs link]
     */
    void NotifyEvent(MouseMoveEvent const& evt);

    /*!
     * \brief Notifies Xu that a window resize event has occured.
     * \sa [Insert windowing event docs link]
     */
    void NotifyEvent(WindowResizeEvent const& evt);

    /*!
     * \brief Processes all events until none are left. After this call, the
     * event queue is empty. \sa [Insert windowing event docs link]
     */
    void ProcessEvents();

    /*!
     * \brief Obtain data necessary to render the UI.
     * \sa [Insert rendering API docs link]
     */
    RenderData const& GetRenderData() const;

    /*!
     * \brief Select which method must be used for event processing.
     * \sa InputReception
     */
    InputReception inputReception;

    /*!
     * \brief Should this be private, and should it be const? Pointer to user's
     * window handler.
     */
    WsiInterface* wsiInterface = nullptr;

    // Temporary
    std::unique_ptr<Widget> root;
    // Adds new root widget
    // In turn calls the user's window handler for a new window.
    WidgetPtr<Widget> AddWindow(const char* title, ISize2 size);

private:
    enum class EventType { MouseMove, WindowResize };

    struct XU_API Event {
        EventType type;
        union {
            MouseMoveEvent mouseMove;
            WindowResizeEvent windowResize;
        } data;
    };

    void DispatchEvent(MouseMoveEvent const& evt);
    void DispatchEvent(WindowResizeEvent const& evt);

    void BuildRenderData();
    void PaintWidgetAndChildren(Widget* widget);

    std::queue<Event> eventQueue;
    RenderData renderData;

    struct WindowData {
        Rect2<int32_t> rect;

        // Some WSI libraries will report cursor enter/leave events out of order
        // so we store a bool per window to account for that.
        bool cursorIsInside;
    };
    struct RootWidgetNode {
        // ID for the windowing interface
        WindowID windowID;
        Surface surface;
        WindowData windowData{};
        std::unique_ptr<Widget> widget;
    };
    std::vector<RootWidgetNode> rootWidgets;

    // Temporary, a single surface, just to get the basic implementation started
    Surface surface;
    // Temporary until proper eventing is implemented
    ISize2 windowSize;
};

} // namespace xu