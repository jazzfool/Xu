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

#include <xu/modules/quick/RenderContext.hpp>

#include <glad/glad.h>
#include <iostream>

namespace xu {

constexpr const char* vtxShader =
R"(#version 430 core
layout(location = 0) in vec2 iPos;

void main() {
    // Since Xu outputs vertices with the Y axis pointing down, we have to invert the y axis for OpenGL.
    // Note that you don't need this wheb building a vulkan renderer.
    vec2 invertY = vec2(iPos.x, 1.0f - iPos.y);
    // Xu outputs vertices in range [0, 1], so we need to transform to OpenGL NDC which is [-1, 1]
    gl_Position = vec4(invertY * 2.0f - 1.0f, 0, 1);
})";

constexpr const char* fragShader = R"(
#version 430 core

layout(location = 0) uniform vec4 color;

out vec4 FragColor;

void main() {
    FragColor = color;
}
)";

RenderContext::RenderContext(LoadProc loadProc) {
    gladLoadGLLoader(loadProc);

    // Setup vertex buffers and VAO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    shaderProgram = CreateShader(vtxShader, fragShader);
}

RenderContext::~RenderContext() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);
}

// TODO: Track old GL state and restore after draw
void RenderContext::RenderDrawData(xu::RenderData const& renderData) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER,
        renderData.vertices.size() * sizeof(xu::Vertex),
        renderData.vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        renderData.indices.size() * sizeof(uint32_t), renderData.indices.data(),
        GL_DYNAMIC_DRAW);

    glBindVertexArray(vao);

    // Pretend we only have a single command list ( = single window) to
    // render, for now.
    xu::CommandList const& cmdList = renderData.cmdLists[0];
    for (xu::CommandList::Iterator it = cmdList.Begin(); it != cmdList.End();
         ++it) {
        if (it->type == xu::DrawCommandType::DrawTriangles) {
            xu::CmdDrawTriangles const& cmd = it->data.drawTriangles;
            auto color = cmd.color.Normalized();
            glUniform4fv(0, 1, color.data());
            glDrawElementsBaseVertex(GL_TRIANGLES, cmd.numIndices,
                GL_UNSIGNED_INT, (void*)(cmd.indexOffset * sizeof(uint32_t)),
                cmd.vertexOffset);
        }
    }
}

unsigned int RenderContext::CreateShader(
    const char* vtxSource, const char* fragSource) {
    unsigned int vtxShader, fragShader;
    vtxShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vtxShader, 1, &vtxSource, nullptr);
    glShaderSource(fragShader, 1, &fragSource, nullptr);

    glCompileShader(vtxShader);
    glCompileShader(fragShader);

    // Now, check for compilation errors
    int success;
    char infolog[512];
    glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vtxShader, 512, nullptr, infolog);
        std::cout << infolog << std::endl;
        return 0;
    }

    // And again for the fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, nullptr, infolog);
        std::cout << infolog << std::endl;
        return 0;
    }

    // Finally, link the vertex and fragment shader together
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vtxShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
        std::cout << infolog << std::endl;
        return 0;
    }

    // These are linked now and can safely be deleted
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}

} // namespace xu