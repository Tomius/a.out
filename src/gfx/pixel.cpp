#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/pixel.hpp"
#include "gl/shader.hpp"

using Gfx::Pixel;

Pixel* Pixel::instance;

Pixel::Pixel()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330\n"
                  "uniform vec2 pos;\n"
                  "uniform mat3 mvp;\n"
                  "void main()\n"
                  "{\n"
                  "    gl_Position = vec4((mvp*vec3(pos,1)).xy, 0, 1);\n"
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
    program.Use();
    program.BindFragDataLocation(0, "col");
    uloc_pos = program.GetUniformLocation("pos");
    uloc_color = program.GetUniformLocation("color");
    uloc_mvp = program.GetUniformLocation("mvp");

    assert(uloc_pos != -1);
    assert(uloc_color != -1);
    assert(uloc_mvp != -1);
}

void Pixel::Draw(glm::vec2 coord, glm::vec4 const& color,
                 glm::mat3 const& mvp)
{
    if (instance == nullptr) {
        instance = new Pixel();
    }

    instance->program.Use();
    instance->vao.Bind();
    gl33::glUniform2fv(instance->uloc_pos, 1, glm::value_ptr(coord));
    gl33::glUniform4fv(instance->uloc_color, 1, glm::value_ptr(color));
    gl33::glUniformMatrix3fv(instance->uloc_mvp, 1,
                             gl33::GL_FALSE, glm::value_ptr(mvp));
    gl33::glDrawArrays(gl33::GL_POINTS, 0, 1);
}
