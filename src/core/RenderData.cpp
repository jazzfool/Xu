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

CommandList::Iterator::Iterator(UnderlyingType it, size_t layer) :
    it(it),
    currentLayer(layer) {}

// TODO: Add debug asserts?
DrawCommand const& CommandList::Iterator::operator*() const { return *it; }
DrawCommand const* CommandList::Iterator::operator->() const { return &*it; }

size_t CommandList::Iterator::CurrentLayer() const { return currentLayer; }
size_t CommandList::Iterator::MergeTarget() const {
    assert(currentLayer != 0 && it->type == DrawCommandType::MergeLayer &&
           "MergeTarget() is only valid when merging layers and the current "
           "layer is not the default layer");
    return currentLayer - 1;
}

CommandList::Iterator& CommandList::Iterator::operator++() {
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

bool CommandList::Iterator::operator==(Iterator const& rhs) const {
    return it == rhs.it;
}

bool CommandList::Iterator::operator!=(Iterator const& rhs) const {
    return it != rhs.it;
}

bool CommandList::Iterator::operator<(Iterator const& rhs) const {
    return it < rhs.it;
}

bool CommandList::Iterator::operator<=(Iterator const& rhs) const {
    return it <= rhs.it;
}

bool CommandList::Iterator::operator>(Iterator const& rhs) const {
    return it > rhs.it;
}

bool CommandList::Iterator::operator>=(Iterator const& rhs) const {
    return it >= rhs.it;
}

size_t CommandList::NumLayers() const {
    size_t layerCount = 1; // Implicit default layer
    for (Iterator it = Begin(); it != End(); ++it) {
        if (it->type == DrawCommandType::NewLayer) { ++layerCount; }
    }
    return layerCount;
}

CommandList::Iterator CommandList::Begin() const {
    return Iterator(commands.begin());
}

CommandList::Iterator CommandList::End() const {
    return Iterator(commands.end());
}

void CommandList::PushCommand(CmdDrawTriangles const& command) {
    DrawCommand cmd;
    cmd.data.drawTriangles = command;
    cmd.type = DrawCommandType::DrawTriangles;
    commands.push_back(cmd);
}

void CommandList::PushCommand(CmdNewLayer const& command) {
    DrawCommand cmd;
    cmd.data.newLayer = command;
    cmd.type = DrawCommandType::NewLayer;
    commands.push_back(cmd);
}

void CommandList::PushCommand(CmdMergeLayer const& command) {
    DrawCommand cmd;
    cmd.data.mergeLayer = command;
    cmd.type = DrawCommandType::MergeLayer;
    commands.push_back(cmd);
}

void RenderData::Clear() {
    cmdLists.clear();
    vertices.clear();
    indices.clear();
}

size_t RenderData::PushVertex(Vertex vertex) {
    vertices.push_back(vertex);
    return vertices.size() - 1;
}
void RenderData::PushIndex(uint32_t index) { indices.push_back(index); }

void RenderData::PushQuad(CommandList& cmdList, FRect2 quad) {
    FVector2 bl = quad.origin;
    FVector2 tl = {quad.origin.x, quad.origin.y + quad.size.y};
    FVector2 br = {quad.origin.x + quad.size.x, quad.origin.y};
    FVector2 tr = quad.origin + quad.size;

    // This is the starting index of the quad we're pushing
    size_t const baseIndex = indices.size();
    size_t const baseVertex = vertices.size();

    size_t const bottomLeftIndex = PushVertex({bl});
    size_t const topLeftIndex = PushVertex({tl});
    size_t const bottomRightIndex = PushVertex({br});
    size_t const topRightIndex = PushVertex({tr});

    // A quad needs 2 triangles, so 6 indices. We will create these in CCW order
    // since this is the most commonly used vertex winding order.

    // First triangle
    PushIndex(bottomLeftIndex);
    PushIndex(bottomRightIndex);
    PushIndex(topRightIndex);
    // Second triangle
    PushIndex(bottomLeftIndex);
    PushIndex(topRightIndex);
    PushIndex(topLeftIndex);

    // Create the draw command
    CmdDrawTriangles command;
    command.indexOffset = baseIndex;
    command.vertexOffset = baseVertex;
    command.numIndices = 6;
    cmdList.PushCommand(command);
}

void RenderData::PushGeometry(CommandList& cmdList, std::vector<Vertex> const& verts, std::vector<uint32_t> const& idx) {
    size_t const baseIndex = indices.size();
    size_t const baseVertex = vertices.size();

    vertices.insert(vertices.end(), verts.begin(), verts.end());
    indices.insert(indices.end(), idx.begin(), idx.end());

    CmdDrawTriangles command;
    command.indexOffset = baseIndex;
    command.vertexOffset = baseVertex;
    command.numIndices = idx.size();
    cmdList.PushCommand(command);
}

} // namespace xu