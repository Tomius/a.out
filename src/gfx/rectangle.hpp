#ifndef UUID_116DAB66_1D6B_11E5_9A21_1697F925EC7B
#define UUID_116DAB66_1D6B_11E5_9A21_1697F925EC7B
#pragma once

#include <glm/vec4.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"

namespace Gfx
{

class Rectangle {
public:
    static void Draw(Rect<float> rect, glm::vec4 const& color);

private:
    static Rectangle* instance;

    Gl::Program program; // todo: shader cache...
    Gl::VertexArray vao;
    Gl::Buffer buffer;

    gl33::GLint uloc_bottom_left;
    gl33::GLint uloc_size;
    gl33::GLint uloc_color;

    Rectangle();
    void SetVbo();
};

}

#endif
