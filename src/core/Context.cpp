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

#include <xu/core/Context.hpp>

#include <iostream> // For debugging.

namespace xu {

void Context::NotifyEvent(WindowResizeEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::WindowResize;
            event.data.windowResize = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

void Context::NotifyEvent(WindowMoveEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::WindowMove;
            event.data.windowMove = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

void Context::NotifyEvent(WindowCursorEnterEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::WindowCursorEnter;
            event.data.windowCursorEnter = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

void Context::NotifyEvent(CursorMoveEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::CursorMove;
            event.data.cursorMove = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

void Context::NotifyEvent(CursorButtonEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::CursorButton;
            event.data.cursorButton = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

void Context::ProcessEvents() {
    if (inputReception == InputReception::Queued) {
        while (!eventQueue.empty()) {
            auto const& evt = eventQueue.front();

            switch (evt.type) {
                case EventType::WindowMove:
                    DispatchEvent(evt.data.windowMove);
                    break;
                case EventType::WindowResize:
                    DispatchEvent(evt.data.windowResize);
                    break;
                case EventType::WindowCursorEnter:
                    DispatchEvent(evt.data.windowCursorEnter);
                    break;
                case EventType::CursorMove:
                    DispatchEvent(evt.data.cursorMove);
                    break;
                case EventType::CursorButton:
                    DispatchEvent(evt.data.cursorButton);
                    break;
            }

            eventQueue.pop();
        }
    }

    BuildRenderData();
}

RenderData const& Context::GetRenderData() const { return renderData; }

void Context::SetTheme(std::unique_ptr<Theme> theme) {
    if (theme) {
        this->theme = std::move(theme);
        InitializeWidgetThemeAndChildren(root.get());
    } else {
        this->theme.reset();
    }
}

Theme* Context::GetTheme() const { return this->theme.get(); }

struct TestWindow : public Widget {
    TestWindow(Context& context) : Widget(context) {}

    virtual FSize2 SizeHint() const override { return {}; }

    virtual void Paint(Surface& surface, Theme* theme) const override {}

    virtual void GenerateTriangles(RenderData& renderData, CommandList& cmdList,
        ISize2 windowSize) const override {}
};

WidgetPtr<Widget> Context::AddWindow(const char* title, ISize2 size) {
    RootWidgetNode newNode{};
    auto newWindowResult = wsiInterface->NewWindow(title, {size.x, size.y});
    newNode.windowID = newWindowResult.id;
    newNode.windowData.rect = newWindowResult.rect;
    newNode.widget = std::unique_ptr<Widget>(new TestWindow(*this));

    rootWidgets.push_back(std::move(newNode));

    return WidgetPtr<Widget>(rootWidgets.back().widget.get());
}

void Context::DispatchEvent(WindowResizeEvent const& evt) {
    auto& rootWidgetNode = *std::find_if(rootWidgets.begin(), rootWidgets.end(),
        [&evt](RootWidgetNode const& node) -> bool {
            return evt.id == node.windowID;
        });

    rootWidgetNode.windowData.rect.size = evt.size;

    windowSize = evt.size;
}

void Context::DispatchEvent(WindowMoveEvent const& evt) {
    auto& rootWidgetNode = *std::find_if(rootWidgets.begin(), rootWidgets.end(),
        [&evt](RootWidgetNode const& node) -> bool {
            return evt.id == node.windowID;
        });

    rootWidgetNode.windowData.rect.origin = evt.position;
}

void Context::DispatchEvent(WindowCursorEnterEvent const& evt) {
    auto& rootWidgetNode = *std::find_if(rootWidgets.begin(), rootWidgets.end(),
        [&evt](RootWidgetNode const& node) -> bool {
            return evt.id == node.windowID;
        });

    rootWidgetNode.windowData.cursorIsInside = evt.entered;
}

void Context::DispatchEvent(CursorMoveEvent const& evt) {
    inputState.cursorPosition = evt.position;
    inputState.cursorPositionDelta = evt.positionDelta;

    // Possibly move down the widget hierarchy
    // and invoke some cursor hover events?
}

void Context::DispatchEvent(CursorButtonEvent const& evt) {
    inputState.SetCursorButton(evt.button, evt.value);

    // Possibly move down the widget hierarchy
    // and invoke some click events if it is also hovering the widget?
}

void Context::BuildRenderData() {
    renderData.Clear();

    renderData.cmdLists.resize(rootWidgets.size());

    /*
    surface.Clear();
    CommandList cmdList;
    PaintWidgetAndChildren(root.get());
    surface.GenerateGeometry(
        renderData, cmdList, FSize2(windowSize.x, windowSize.y));
        */

    for (size_t i = 0; i < rootWidgets.size(); i++) {
        RootWidgetNode& window = rootWidgets[i];
        CommandList& cmdList = renderData.cmdLists[i];

        PaintWidgetAndChildren(window.widget.get());

        window.surface.Clear();
        surface.GenerateGeometry(renderData, cmdList,
            FSize2{(float)window.windowData.rect.size.x,
                (float)window.windowData.rect.size.y});
    }
    /*
    // Temporarily use Widget::GenerateTriangles
    auto triangleGen = [&cmdList, this](auto self, Widget* widget) -> void {
        widget->GenerateTriangles(renderData, cmdList, windowSize);
        for (size_t child = 0; child < widget->NumChildren(); ++child) {
            self(self, widget->GetChild(child));
        }
    };
    // triangleGen(triangleGen, root.get());
    */

    // renderData.cmdLists.push_back(std::move(cmdList));
    // renderData.cmdLists.push_back(std::move(cmdList));
}

void Context::PaintWidgetAndChildren(Widget* widget) {
    if (widget->hidden) { return; }

    widget->Paint(surface, theme.get());

    for (size_t child = 0; child < widget->NumChildren(); ++child) {
        PaintWidgetAndChildren(widget->GetChild(child));
    }
}

void Context::InitializeWidgetThemeAndChildren(Widget* widget) {
    widget->InitializeTheme(*theme.get());
    for (size_t child = 0; child < widget->NumChildren(); ++child) {
        InitializeWidgetThemeAndChildren(widget->GetChild(child));
    }
}

} // namespace xu