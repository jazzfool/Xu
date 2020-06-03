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

#include <xu/core/Surface.hpp>

#include "Tessellation.hpp"

namespace xu {

void Surface::Paint(VectorPath const& geometry) { paths.push_back(geometry); }

void Surface::Clear() { paths.clear(); }

void Surface::GenerateGeometry(
    RenderData& renderData, CommandList& cmdList, FSize2 windowSize) {
    for (auto const& path : paths) {
        auto points = FlattenPath(path, 1.0f);
        auto indices = Triangulate(points);

        // TODO: A little inefficient
        std::vector<Vertex> vertices;
        vertices.reserve(points.size());
        for (auto const pt : points) { 
            Vertex vtx;
            // Sometimes causes strange transformations
            vtx.position.x = pt.x / windowSize.x;
            vtx.position.y = pt.y / windowSize.y;
            vertices.push_back(vtx); 
        }

        renderData.PushGeometry(cmdList, vertices, indices);
    }
}

} // namespace xu