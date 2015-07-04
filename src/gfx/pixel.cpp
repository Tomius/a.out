#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/pixel.hpp"
#include "gl/shader.hpp"

using Gfx::Pixel;

Pixel* Pixel::instance;

Pixel::Pixel()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330\n"
                  "in float ignored;\n"
                  "uniform vec2 pos;\n"
                  "void main()\n"
                  "{\n"
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
    auto pos = program.GetAttribLocation("ignored");
    gl33::glEnableVertexAttribArray(pos);
    gl33::glVertexAttribPointer(pos, 2, gl33::GL_FLOAT, gl33::GL_FALSE, 0, 0);

    program.Use();
    program.BindFragDataLocation(0, "col");
    uloc_pos = program.GetUniformLocation("pos");
    uloc_color = program.GetUniformLocation("color");

    assert(uloc_pos != -1);
    assert(uloc_color != -1);
}

void Pixel::Draw(glm::vec2 coord, glm::vec4 const& color)
{
    if (instance == nullptr) {
        instance = new Pixel();
    }

    instance->program.Use();
    instance->vao.Bind();
    gl33::glUniform2fv(instance->uloc_pos, 1, glm::value_ptr(coord));
    gl33::glUniform4fv(instance->uloc_color, 1, glm::value_ptr(color));
    gl33::glDrawArrays(gl33::GL_POINTS, 0, 1);
}

void Pixel::SetVbo()
{
    gl33::GLfloat cbuf[1]{};
    buffer.Bind(gl33::GL_ARRAY_BUFFER);
    gl33::glBufferData(gl33::GL_ARRAY_BUFFER, sizeof(cbuf), cbuf,
                       gl33::GL_STATIC_DRAW);
}
