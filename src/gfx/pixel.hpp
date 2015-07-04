#pragma once

#include <glm/vec4.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"

namespace Gfx
{

class Pixel {
public:
    static void Draw(glm::vec2 coord, glm::vec4 const& color);

private:
    static Pixel* instance;

    Gl::Program program;

    gl33::GLint uloc_pos;
    gl33::GLint uloc_color;

    Pixel();
};

}
