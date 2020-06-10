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
#include <xu/core/Point2.hpp>
#include <xu/core/Vector2.hpp>
#include <xu/core/Events.hpp>
#include <xu/core/RenderData.hpp>
#include <xu/core/Surface.hpp>
#include <xu/core/WsiInterface.hpp>
#include <xu/core/InputState.hpp>
#include <xu/core/Theme.hpp>

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
     * \brief Notifies Xu that a window resize event has occured.
     * \sa [Insert windowing event docs link]
     */
    void NotifyEvent(WindowResizeEvent const& evt);
    /*!
     * \brief Notifies Xu that a window resize event has occured.
     * \sa [Insert windowing event docs link]
     */
    void NotifyEvent(WindowMoveEvent const& evt);
    /*!
     * \brief Notifies Xu that a cursor has entered/left a window.
     * \sa [Insert windowing event docs link]
     */
    void NotifyEvent(WindowCursorEnterEvent const& evt);
    /*!
     * \brief Notifies Xu that a cursor move event has occured.
     * \sa [Insert windowing event docs link]
     */
    void NotifyEvent(CursorMoveEvent const& evt);
    /*!
     * \brief Notifies Xu that a cursor button event has occurred.
     * \sa [Insert windowing event docs link]
     */
    void NotifyEvent(CursorButtonEvent const& evt);


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
     * \brief Changes the theme that should be given to widgets during
     * rendering.
     *
     * \param theme The theme to change to. If nullptr is given, the theme
     * mechanics are disabled.
     */
    void SetTheme(std::unique_ptr<Theme> theme = nullptr);

    /*!
     * \brief Returns the theme that would be given to widgets during
     * rendering. Possibly nullptr.
     */
    Theme* GetTheme() const;

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

    // Adds new root widget
    // In turn calls the user's window handler for a new window.
    WidgetPtr<Widget> AddWindow(const char* title, ISize2 size);

private:
    enum class EventType { 
        WindowResize,
        WindowMove,
        WindowCursorEnter,
        CursorMove,
        CursorButton,
    };
    struct XU_API Event {
        EventType type;
        union {
            WindowResizeEvent windowResize;
            WindowMoveEvent windowMove;
            WindowCursorEnterEvent windowCursorEnter;
            CursorMoveEvent cursorMove;
            CursorButtonEvent cursorButton;
        } data;
    };
    void DispatchEvent(WindowResizeEvent const& evt);
    void DispatchEvent(WindowMoveEvent const& evt);
    void DispatchEvent(WindowCursorEnterEvent const& evt);
    void DispatchEvent(CursorMoveEvent const& evt);
    void DispatchEvent(CursorButtonEvent const& evt);
    std::queue<Event> eventQueue;

    void DoWidgetCallbacks();
    void BuildRenderData();
    void PaintWidgetAndChildren(Widget* widget, Surface& surface);
    void InitializeWidgetThemeAndChildren(Widget* widget);

    RenderData renderData;

    std::unique_ptr<Theme> theme;

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

    // Temporary?
    InputState inputState;
    InputState prevInputState;
   
    // Temporary until proper eventing is implemented
    ISize2 windowSize;
};

} // namespace xu