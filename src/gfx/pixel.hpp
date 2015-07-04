#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "gl/shader.hpp"
#include "gl/buffer.hpp"

namespace Gfx
{

class Pixel {
public:
    static void Draw(glm::vec2 coord, glm::vec4 const& color);

private:
    static Pixel* instance;

    Gl::Program program;
    Gl::VertexArray vao;

    gl33::GLint uloc_pos;
    gl33::GLint uloc_color;

    Pixel();
};

}
