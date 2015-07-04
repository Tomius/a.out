#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "gl/shader.hpp"
#include "gl/buffer.hpp"

namespace Gfx
{

class Line {
public:
    static void Draw(glm::vec2 coord1, glm::vec2 coord2,
                     glm::vec4 const& color);

private:
    static Line* instance;

    Gl::Program program;
    Gl::VertexArray vao;

    gl33::GLint uloc_pos0, uloc_pos1;
    gl33::GLint uloc_color;

    Line();
};

}
