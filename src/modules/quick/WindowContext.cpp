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

#include <xu/modules/quick/WindowContext.hpp>

#include <GLFW/glfw3.h>

namespace xu::quick {

static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    Context* ctx = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));

    CursorMoveEvent evt;
    evt.position.x = xpos;
    evt.position.y = ypos;
    ctx->NotifyEvent(evt);
}

static void MouseButtonCallback(
    GLFWwindow* window, int button, int state, int modifier) {
    Context* ctx = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
    CursorButtonEvent evt;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        evt.button = CursorButton::Primary;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        evt.button = CursorButton::Secondary;
    }

    evt.value = (state == GLFW_PRESS);
    ctx->NotifyEvent(evt);
}

static void WindowResizeCallback(GLFWwindow* window, int w, int h) {
    Context* ctx = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
    WindowResizeEvent evt;
    evt.size.x = w;
    evt.size.y = h;
    evt.id = static_cast<WindowID>(reinterpret_cast<uint64_t>(window));
    ctx->NotifyEvent(evt);
}

WindowContext::WindowContext(Context& ctx) {
    xuCtx = &ctx;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

WindowContext::~WindowContext() {
    DestroyWindows();
    glfwTerminate();
}

LoadProc WindowContext::GetLoadProc() const {
    return (LoadProc)glfwGetProcAddress;
}

WindowContext::NewWindowResult WindowContext::NewWindow(
    char const* title, ISize2 extent) {
    GLFWwindow* win
        = glfwCreateWindow(extent.x, extent.y, title, nullptr, nullptr);
    glfwSetWindowUserPointer(win, xuCtx);
    glfwMakeContextCurrent(win);
    glfwSetWindowSizeCallback(win, &WindowResizeCallback);
    glfwSetCursorPosCallback(win, &MouseMoveCallback);
    glfwSetMouseButtonCallback(win, &MouseButtonCallback);
    windows.push_back(win);
    NewWindowResult result;
    result.id = static_cast<WindowID>(reinterpret_cast<uint64_t>(win));
    glfwGetWindowPos(win, &result.rect.origin.x, &result.rect.origin.y);
    result.rect.size = extent;
    return result;
}

void WindowContext::DestroyWindow(WindowID id) {
    GLFWwindow* window
        = reinterpret_cast<GLFWwindow*>(static_cast<uint64_t>(id));
    windows.erase(std::find(windows.begin(), windows.end(), window));
    glfwDestroyWindow(window);
}

WindowID WindowContext::GetMainWindow() {
    return static_cast<WindowID>(reinterpret_cast<uint64_t>(windows.front()));
}

void WindowContext::PollEvents() { glfwPollEvents(); }

bool WindowContext::ShouldClose(WindowID id) const {
    GLFWwindow* window
        = reinterpret_cast<GLFWwindow*>(static_cast<uint64_t>(id));
    return glfwWindowShouldClose(window);
}

void WindowContext::SwapBuffers(WindowID id) {
    GLFWwindow* window
        = reinterpret_cast<GLFWwindow*>(static_cast<uint64_t>(id));
    glfwSwapBuffers(window);
}

void WindowContext::DestroyWindows() {
    for (auto window : windows) { glfwDestroyWindow((GLFWwindow*)window); }
    windows.clear();
}

} // namespace xu::quick