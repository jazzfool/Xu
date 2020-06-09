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

#include <xu/core/Color.hpp>
#include <xu/core/Definitions.hpp>
#include <xu/core/RenderData.hpp>
#include <xu/core/Size2.hpp>
#include <xu/core/VectorPath.hpp>

namespace xu {

/*!
 * \brief Represents a surface for widgets to paint their visual representation
 * on.
 */
class XU_API Surface {
public:
    // TODO: More paint options for coloring, etc
    void Paint(BakedVectorPath const& geometry, Color const& color);

    void Clear();

private:
    friend class Context;

    void GenerateGeometry(
        RenderData& renderData, CommandList& cmdList, FSize2 windowSize);

    struct PaintNode {
        BakedVectorPath path;
        Color color;
    };

    std::vector<PaintNode> paintNodes;
};

} // namespace xu