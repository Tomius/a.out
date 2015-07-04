#pragma once

#include <glm/vec4.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"

namespace Gfx
{

class Circle {
public:
    static void Draw(glm::vec2 center, float radius, glm::vec4 const& color);

private:
    static Circle* instance;

    Gl::Program program;
    Gl::VertexArray vao;

    gl33::GLint uloc_center;
    gl33::GLint uloc_radius;
    gl33::GLint uloc_color;

    Circle();
    void SetVbo();
};

}
