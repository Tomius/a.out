#pragma once

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"

namespace Gfx
{

class Circle {
public:
    static void Draw(glm::vec2 center, float radius, glm::vec4 const& color,
                     glm::mat3 const& mvp = glm::mat3{});

private:
    Gl::Program program;
    Gl::VertexArray vao;

    gl33::GLint uloc_center;
    gl33::GLint uloc_radius;
    gl33::GLint uloc_color;
    gl33::GLint uloc_mvp;

    Circle();

    static Circle& GetInstance();
};

}
