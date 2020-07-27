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

#include <vector>

#include <xu/core/WsiInterface.hpp>
#include <xu/core/Context.hpp>
#include <xu/modules/quick/LoadProc.hpp>

namespace xu::quick {

// GLFW-based windowing manager for Xu
class WindowContext : public WsiInterface {
public:
    WindowContext(Context& ctx);
    ~WindowContext();

    LoadProc GetLoadProc() const;

    NewWindowResult NewWindow(char const* title, ISize2 extent) override;
    void DestroyWindow(WindowID id) override;
    void DestroyWindows();

    WindowID GetMainWindow();

    void PollEvents();
    bool ShouldClose(WindowID id) const;
    void SwapBuffers(WindowID id);

private:
    std::vector<void*> windows;
    Context* xuCtx;
};

} // namespace xu::quick