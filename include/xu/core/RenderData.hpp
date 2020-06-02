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
#include <xu/core/Definitions.hpp>
#include <xu/core/Vector2.hpp>
#include <xu/core/Rect2.hpp>

namespace xu {

/*!
 * \brief One vertex used in the draw commands. This structure is guaranteed to
 * be a POD, and is memcpy()'able into vertex buffers.
 */
struct XU_API Vertex {
    /*!
     * \brief Position of the vertex. Coordinate space etc to be decided. TODO
     */
    FVector2 position;
};

/*!
 * \brief Used to describe which kinda of draw command is active in the command
 * union \sa [Insert rendering API doc link]
 */
enum class DrawCommandType {
    /*!
     * \brief Indicates that a new layer must be created, pushed onto the layer
     * stack and used for subsequent drawing operations
     */
    NewLayer,
    /*!
     * \brief Indicates that the current layer must be merged with the previous
     * layer.
     */
    MergeLayer,
    /*!
     * \brief Indicates that this command is a drawcommand that draws triangles.
     */
    DrawTriangles
};

/*!
 * \brief Indicates the filter mode to be used when merging layers.
 */
enum class LayerFilter {
    /*!
     * \brief No filtering mode. Equivalent to doing a blit.
     */
    None,
    /*!
     * \brief Blur filter. Used to achieve effects like backdrop blur.
     */
    Blur,
    /*!
     * \brief Color matrix filter. Used to achieve color transformations (e.g.
     * inversion or saturation).
     */
    ColorMatrix
};

union LayerFilterInfo {
    /*!
     * \brief Blur radius to apply when merging layers with a blur filter.
     *
     * \sa LayerFilter::Blur
     */
    FVector2 blurSigma;
    /*!
     * \brief Color matrix (4 row, 5 col) to apply when merging layers with a
     * color matrix filter.
     *
     * \sa Layerfilter::ColorMatrix
     */
    float colorMatrix[4][5]; // TODO: matrix (4x5) class.
    // Note for shaders: color must be converted to a 5x1 matrix first (in the
    // form [R | G | B | A | 1]).
};

/*!
 * \brief Structure describing a DrawTriangles command.
 * \sa [Rendering API]
 */
struct XU_API CmdDrawTriangles {
    /*!
     * \brief Start index in the vertices array
     */
    size_t vertexOffset;
    /*!
     * \brief Start index in the indices array
     */
    size_t indexOffset;
    /*!
     * \brief Amount of indices to be rendered
     */
    size_t numIndices;
};

/*!
 * \brief Structure describing a NewLayer command.
 * \sa [Rendering API]
 */
struct XU_API CmdNewLayer {};

/*!
 * \brief Structure describing a MergeLayer command.
 * \sa [Rendering API]
 */
struct XU_API CmdMergeLayer {
    LayerFilter filter;
    LayerFilterInfo filterInfo;
};

/*!
 * \brief Structure describing a draw command.
 * \sa [Rendering API]
 */
struct XU_API DrawCommand {
    /*!
     * \brief Indicates which type of the data union is active.
     */
    DrawCommandType type;
    /*!
     * \brief Stores data necessary for this draw command. Use the type member
     * to determine the correct member
     */
    union {
        CmdDrawTriangles drawTriangles;
        CmdNewLayer newLayer;
        CmdMergeLayer mergeLayer;
    } data;
};

/*!
 * \brief Stores a list of drawing commands. Each OS window gets its own command
 * list.
 * \sa [Rendering API]
 */
class XU_API CommandList {
public:
    /*! \brief Iterator that can iterate over the drawcommands in the
     * CommandList, keeping track of things like the current layer, vertex
     * offset, etc. This class satisfies the BidirectionalIterator concept
     */
    class XU_API Iterator {
    public:
        using UnderlyingType = std::vector<DrawCommand>::const_iterator;

        Iterator() = default;
        Iterator(UnderlyingType it, size_t layer = 0);

        Iterator(Iterator const&) = default;
        Iterator& operator=(Iterator const&) = default;

        /*!
         * \brief Access the underlying draw command
         */
        DrawCommand const& operator*() const;
        /*!
         * \brief Access the underlying draw command
         */
        DrawCommand const* operator->() const;

        /*! \brief Obtain the index of the current drawing layer. When the
         * current draw command type is NewLayer or MergeLayer, the old layer is
         * returned and the next iterator will have an updated layer index.
         */
        size_t CurrentLayer() const;

        /*! \brief Returns the index of the layer to merge with. Only valid if
         * the draw command type is MergeLayer and the current layer is not the
         * default layer. Equivalent to CurrentLayer() - 1.
         */
        size_t MergeTarget() const;

        Iterator& operator++();
        Iterator operator++(int);

        Iterator operator--();
        Iterator operator--(int);

        bool operator==(Iterator const& rhs) const;
        bool operator!=(Iterator const& rhs) const;

        bool operator<(Iterator const& rhs) const;
        bool operator<=(Iterator const& rhs) const;

        bool operator>(Iterator const& rhs) const;
        bool operator>=(Iterator const& rhs) const;

    private:
        UnderlyingType it;
        size_t currentLayer = 0;
    };

    /*! \brief Obtain the total amount of layers in this RenderData structure.
     * Useful to allocate resources upfront before rendering.
     */
    size_t NumLayers() const;

    Iterator Begin() const;
    Iterator End() const;

    /*! \brief Push a new command into the command list.
     *  \param command Command to push into the list
     */
    void PushCommand(CmdDrawTriangles const& command);
    /*! \brief Push a new command into the command list.
     *  \param command Command to push into the list
     */
    void PushCommand(CmdNewLayer const& command);
    /*! \brief Push a new command into the command list.
     *  \param command Command to push into the list
     */
    void PushCommand(CmdMergeLayer const& command);

private:
    friend class Context;

    std::vector<DrawCommand> commands;
};

/*!
 * \brief Main class that stores all data needed to render the full UI.
 * Maintains a xu::CommandList for each OS window.
 */
class XU_API RenderData {
public:
    // We will build one command list for each OS window so they can be executed
    // in parallel for people who build a vulkan renderer. This also avoids the
    // hassle of having CommandList::Iterator track the current window to draw
    // to.
    std::vector<CommandList> cmdLists;
    /*!
     * \brief List of all vertices used by all command lists. These vertices are in window coordinates.
     * To use these in your shader you have to apply the correct projection matrix first (typically an orthographic projection).
     */
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    /*!
     * \brief Clears all render data stored.
     */
    void Clear();

    /*!
     * \brief Adds a single vertex to the vertex list and returns its index
     */
    size_t PushVertex(Vertex vertex);

    /*!
     * \brief Adds a single index to the index list.
     */
    void PushIndex(uint32_t index);

    /*!
     * \brief Adds a single quad to the index and vertex list. Also adds a
     * drawcommand for it. It is not recommended to use this function to create
     * complex geometry, since multiple quads should be condensed into A single
     * drawcall.
     */
    void PushQuad(CommandList& cmdList, FRect2 quad);
};

} // namespace xu