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

#include <xu/core/RenderData.hpp>

#include <cassert>

namespace xu {

CommandList::Iterator::Iterator(UnderlyingType it, size_t layer)
    : it(it), currentLayer(layer) {}

// TODO: Add debug asserts?
DrawCommand const &CommandList::Iterator::operator*() const { return *it; }
DrawCommand const *CommandList::Iterator::operator->() const { return &*it; }

size_t CommandList::Iterator::CurrentLayer() const { return currentLayer; }
size_t CommandList::Iterator::MergeTarget() const {
    assert(currentLayer != 0 && it->type == DrawCommandType::MergeLayer &&
           "MergeTarget() is only valid when merging layers and the current "
           "layer is not the default layer");
    return currentLayer - 1;
}

CommandList::Iterator &CommandList::Iterator::operator++() {
    if (it->type == DrawCommandType::NewLayer) {
        ++currentLayer;
    } else if (it->type == DrawCommandType::MergeLayer) {
        --currentLayer;
    }
    ++it;
    return *this;
}

CommandList::Iterator CommandList::Iterator::operator++(int) {
    Iterator copy = *this;
    ++(*this);
    return copy;
}

CommandList::Iterator CommandList::Iterator::operator--() {
    if (it->type == DrawCommandType::NewLayer) {
        --currentLayer;
    } else if (it->type == DrawCommandType::MergeLayer) {
        ++currentLayer;
    }
    --it;
    return *this;
}

CommandList::Iterator CommandList::Iterator::operator--(int) {
    Iterator copy = *this;
    --(*this);
    return copy;
}

bool CommandList::Iterator::operator==(Iterator const &rhs) const {
    return it == rhs.it;
}

bool CommandList::Iterator::operator!=(Iterator const &rhs) const {
    return it != rhs.it;
}

bool CommandList::Iterator::operator<(Iterator const &rhs) const {
    return it < rhs.it;
}

bool CommandList::Iterator::operator<=(Iterator const &rhs) const {
    return it <= rhs.it;
}

bool CommandList::Iterator::operator>(Iterator const &rhs) const {
    return it > rhs.it;
}

bool CommandList::Iterator::operator>=(Iterator const &rhs) const {
    return it >= rhs.it;
}

size_t CommandList::NumLayers() const {
    size_t layerCount = 1; // Implicit default layer
    for (Iterator it = Begin(); it != End(); ++it) {
        if (it->type == DrawCommandType::NewLayer) {
            ++layerCount;
        }
    }
    return layerCount;
}

CommandList::Iterator CommandList::Begin() const {
    return Iterator(commands.begin());
}

CommandList::Iterator CommandList::End() const {
    return Iterator(commands.end());
}

void CommandList::PushCommand(CmdDrawTriangles const &command) {
    DrawCommand cmd;
    cmd.data.drawTriangles = command;
    cmd.type = DrawCommandType::DrawTriangles;
    commands.push_back(cmd);
}

void CommandList::PushCommand(CmdNewLayer const &command) {
    DrawCommand cmd;
    cmd.data.newLayer = command;
    cmd.type = DrawCommandType::NewLayer;
    commands.push_back(cmd);
}

void CommandList::PushCommand(CmdMergeLayer const &command) {
    DrawCommand cmd;
    cmd.data.mergeLayer = command;
    cmd.type = DrawCommandType::MergeLayer;
    commands.push_back(cmd);
}

} // namespace xu