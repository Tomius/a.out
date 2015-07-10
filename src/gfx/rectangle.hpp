#pragma once

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"

namespace Gfx
{

class Rectangle {
public:
    static void Draw(Rect<float> rect, glm::vec4 const& color,
                     glm::mat3 const& mvp = glm::mat3{});

    static void Draw(Rect<float> rect, Gl::Texture& tex,
                     const glm::vec4& color = glm::vec4{1,1,1,1},
                     const glm::mat3& mvp = glm::mat3{});

private:
    static Rectangle* instance;

    Gl::Program program;
    Gl::VertexArray vao;
    Gl::Texture tex;

    gl33::GLint uloc_bottom_left;
    gl33::GLint uloc_size;
    gl33::GLint uloc_color;
    gl33::GLint uloc_mvp;
    gl33::GLint uloc_tex;

    Rectangle();
};

}
