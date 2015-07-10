#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include "gl/shader.hpp"
#include "gl/buffer.hpp"

namespace Gfx
{

class Pixel {
public:
    static void Draw(glm::vec2 coord, glm::vec4 const& color,
                     glm::mat3 const& mvp = glm::mat3{});

private:
    Gl::Program program;
    Gl::VertexArray vao;

    gl33::GLint uloc_pos;
    gl33::GLint uloc_color;
    gl33::GLint uloc_mvp;

    Pixel();

    static Pixel& GetInstance();
};

}
