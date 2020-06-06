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

namespace xu {

void Context::NotifyEvent(MouseMoveEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::MouseMove;
            event.data.mouseMove = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

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

void Context::ProcessEvents() {
    if (inputReception == InputReception::Queued) {
        while (!eventQueue.empty()) {
            const auto& evt = eventQueue.front();

            switch (evt.type) {
                case EventType::MouseMove:
                    DispatchEvent(evt.data.mouseMove);
                    break;
                case EventType::WindowResize:
                    DispatchEvent(evt.data.windowResize);
                    break;
            }

            eventQueue.pop();
        }
    }

    BuildRenderData();
}

RenderData const& Context::GetRenderData() const { return renderData; }

struct TestWindow : public Widget {
    TestWindow() : Widget(nullptr) {}

    virtual FSize2 SizeHint() const override { return {}; }

    virtual void Paint(Surface& surface, Theme* theme) const override {}

    virtual void GenerateTriangles(RenderData& renderData, CommandList& cmdList,
        ISize2 windowSize) const override {}
};

WidgetPtr<Widget> Context::AddWindow(const char* title, ISize2 size) {
    RootWidgetNode newNode{};
    auto newWindowResult = wsiInterface->NewWindow(
        "Xu test window, remember to implement window title", {500, 500});
    newNode.windowID = newWindowResult.id;
    newNode.windowData.rect = newWindowResult.rect;
    newNode.widget = std::unique_ptr<Widget>(new TestWindow);

    rootWidgets.push_back(std::move(newNode));

    return WidgetPtr<Widget>(rootWidgets.back().widget.get());
}

void Context::DispatchEvent(MouseMoveEvent const& evt) {}

void Context::DispatchEvent(WindowResizeEvent const& evt) {
    windowSize = evt.size;
}

void Context::BuildRenderData() {
    renderData.Clear();
    surface.Clear();
    CommandList cmdList;
    PaintWidgetAndChildren(root.get());
    surface.GenerateGeometry(
        renderData, cmdList, FSize2(windowSize.x, windowSize.y));

    // Temporarily use Widget::GenerateTriangles
    auto triangleGen = [&cmdList, this](auto self, Widget* widget) -> void {
        widget->GenerateTriangles(renderData, cmdList, windowSize);
        for (size_t child = 0; child < widget->NumChildren(); ++child) {
            self(self, widget->GetChild(child));
        }
    };
    // triangleGen(triangleGen, root.get());

    renderData.cmdLists.push_back(std::move(cmdList));
}

void Context::PaintWidgetAndChildren(Widget* widget) {
    if (widget->hidden) { return; }

    widget->Paint(surface, theme.get());

    for (size_t child = 0; child < widget->NumChildren(); ++child) {
        PaintWidgetAndChildren(widget->GetChild(child));
    }
}

} // namespace xu