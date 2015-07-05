#pragma once

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"

namespace Gfx
{

class Rectangle {
public:
    static void Draw(Rect<float> rect, glm::vec4 const& color,
                     glm::mat3 const& mvp = glm::mat3{});

private:
    static Rectangle* instance;

    Gl::Program program;
    Gl::VertexArray vao;

    gl33::GLint uloc_bottom_left;
    gl33::GLint uloc_size;
    gl33::GLint uloc_color;
    gl33::GLint uloc_mvp;

    Rectangle();
};

}
