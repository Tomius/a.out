#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/rectangle.hpp"
#include "gl/shader.hpp"
#include <iostream>

using Gfx::Rectangle;

Rectangle* Rectangle::instance;

Rectangle::Rectangle()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330\n"
                  "in vec2 position;\n"
                  "uniform vec2 bottom_left;\n"
                  "uniform vec2 size;\n"
                  "void main()\n"
                  "{\n"
                  "    vec2 pos = bottom_left + size*position;\n"
                  "    gl_Position = vec4(pos, 0, 1);\n"
                  "}\n"
              },
              Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                  "#version 330\n"
                  "out vec4 col;\n"
                  "uniform vec4 color;\n"
                  "void main()\n"
                  "{\n"
                  "    col = color;\n"
                  "}"
              }}
{
    SetVbo();
    vao.Bind();
    auto pos = program.GetAttribLocation("position");
    gl33::glEnableVertexAttribArray(pos);
    gl33::glVertexAttribPointer(pos, 2, gl33::GL_FLOAT, gl33::GL_FALSE, 0, 0);

    program.Use();
    program.BindFragDataLocation(0, "col");
    uloc_bottom_left = program.GetUniformLocation("bottom_left");
    uloc_size = program.GetUniformLocation("size");
    uloc_color = program.GetUniformLocation("color");
}

void Rectangle::Draw(Rect<float> rect, glm::vec4 const& color)
{
    if (instance == nullptr) {
        instance = new Rectangle();
    }

    instance->program.Use();
    instance->vao.Bind();
    gl33::glUniform2fv(instance->uloc_bottom_left, 1, &rect.x);
    gl33::glUniform2fv(instance->uloc_size, 1, &rect.width);
    gl33::glUniform4fv(instance->uloc_color, 1, glm::value_ptr(color));
    gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
}

void Rectangle::SetVbo()
{
    gl33::GLfloat cbuf[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0,
    };
    buffer.Bind(gl33::GL_ARRAY_BUFFER);
    gl33::glBufferData(gl33::GL_ARRAY_BUFFER, sizeof(cbuf), cbuf,
                       gl33::GL_STATIC_DRAW);
}
