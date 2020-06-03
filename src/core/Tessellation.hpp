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

#include <xu/core/Point2.hpp>
#include <xu/core/VectorPath.hpp>
#include <xu/core/Definitions.hpp>

#include <vector>
#include <utility>

namespace xu {

// We would want to expose these types to the user so that they can
// customize how the stroked path looks.

enum class LineCap { Butt, Square };
enum class LineJoin { Bevel, Miter };

// This is the first step in the vector path triangulation process.
//
// This essentially "flattens" the path. That is to say; it converts bezier
// curves (think of quadratics, cubics) into a series of lines
// (points). This is a simple process of plugging in evenly spaced values, much
// like plugging in x into a function to get back y. (Note that these
// are not perfectly evenly spaced; a smarter algorithm is used to decide how to
// space out the points).
//
// The quality parameter decided the degree of spacing; i.e. how detailed the
// path flattening should be. If it is too low, the final path will appear
// jagged and low quality.
std::vector<FPoint2> FlattenPath(VectorPath const& path, double quality);

// This is an optional step in the vector path triangulation process; right
// after flattening.
//
// There are many cases where you want to render the "stroke" (i.e. the outline)
// of a path instead of filling it solid. This will generate a new set of points
// which are expanded outwards on the miter normals and end cap mode which will
// achieve the effect of a stroke.
//
// strokeWidth is the thickness of the stroke.
// cap is how the end of lines should appear.
// quality only applies to LineCap::Round and controls how smooth the rounded
// cap should appear.
//
// Also, this should be the final step since it also generates indices during
// stroke expansion.
std::pair<std::vector<FPoint2>, std::vector<uint32_t>> ExpandStroke(
    std::vector<FPoint2> const& polygon, float strokeWidth, LineCap cap,
    LineJoin join, float miterLimit, double quality);

// This is the final step in the vector path triangulation process.
//
// It uses a triangulation algorithm which will return a list of indices.
//
// After invoking this, you should now have;
// - a list of points
// - a list of indices
//
// Now you put the points in your vertex type and upload to a vertex buffer, and
// upload the indices to an index buffer; then render. :)
std::vector<uint32_t> Triangulate(std::vector<FPoint2> const& polygon);

} // namespace xu
