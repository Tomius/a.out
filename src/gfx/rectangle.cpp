#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/rectangle.hpp"
#include "gl/shader.hpp"

using Gfx::Rectangle;

Rectangle* Rectangle::instance;

Rectangle::Rectangle()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330 core\n"
                  "const vec2 data[4] = vec2[](\n"
                  "    vec2(0, 0),\n"
                  "    vec2(0, 1),\n"
                  "    vec2(1, 1),\n"
                  "    vec2(1, 0)\n"
                  ");\n"
                  "uniform vec2 bottom_left;\n"
                  "uniform vec2 size;\n"
                  "uniform mat3 mvp;\n"
                  "void main()\n"
                  "{\n"
                  "    vec2 pos = bottom_left + size*data[gl_VertexID];\n"
                  "    gl_Position = vec4((mvp*vec3(pos,1)).xy, 0, 1);\n"
                  "}\n"
              },
              Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                  "#version 330 core\n"
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
    uloc_bottom_left = program.GetUniformLocation("bottom_left");
    uloc_size = program.GetUniformLocation("size");
    uloc_color = program.GetUniformLocation("color");
    uloc_mvp = program.GetUniformLocation("mvp");

    assert(uloc_bottom_left != -1);
    assert(uloc_size != -1);
    assert(uloc_color != -1);
    assert(uloc_mvp != -1);
}

void Rectangle::Draw(Rect<float> rect, glm::vec4 const& color,
                     glm::mat3 const& mvp)
{
    if (instance == nullptr) {
        instance = new Rectangle();
    }

    instance->program.Use();
    instance->vao.Bind();
    gl33::glUniform2fv(instance->uloc_bottom_left, 1, &rect.x);
    gl33::glUniform2fv(instance->uloc_size, 1, &rect.width);
    gl33::glUniform4fv(instance->uloc_color, 1, glm::value_ptr(color));
    gl33::glUniformMatrix3fv(instance->uloc_mvp, 1,
                             gl33::GL_FALSE, glm::value_ptr(mvp));
    gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
}
