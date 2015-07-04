#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/circle.hpp"
#include "gl/shader.hpp"

using Gfx::Circle;

Circle* Circle::instance;

Circle::Circle()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330 core\n"
                  "const vec2 data[4] = vec2[](\n"
                  "    vec2(-1, -1),\n"
                  "    vec2(-1, +1),\n"
                  "    vec2(+1, +1),\n"
                  "    vec2(+1, -1)\n"
                  ");\n"
                  "uniform vec2 center;\n"
                  "uniform float radius;\n"
                  "out vec2 pos;\n"
                  "void main()\n"
                  "{\n"
                  "    pos = center + radius*data[gl_VertexID];\n"
                  "    gl_Position = vec4(pos, 0, 1);\n"
                  "}\n"
              },
              Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                  "#version 330 core\n"
                  "in vec2 pos;\n"
                  "out vec4 col;\n"
                  "uniform vec4 color;\n"
                  "uniform vec2 center;\n"
                  "uniform float radius;\n"
                  "void main()\n"
                  "{\n"
                  "    if (length(pos - center) > radius) discard;\n"
                  "    col = color;\n"
                  "}"
              }}
{
    program.Use();
    program.BindFragDataLocation(0, "col");
    uloc_center = program.GetUniformLocation("center");
    uloc_radius = program.GetUniformLocation("radius");
    uloc_color = program.GetUniformLocation("color");

    assert(uloc_center != -1);
    assert(uloc_radius != -1);
    assert(uloc_color != -1);
}

void Circle::Draw(glm::vec2 center, float radius, glm::vec4 const& color)
{
    if (instance == nullptr) {
        instance = new Circle();
    }

    instance->program.Use();
    instance->vao.Bind();
    gl33::glUniform2fv(instance->uloc_center, 1, glm::value_ptr(center));
    gl33::glUniform1f(instance->uloc_radius, radius);
    gl33::glUniform4fv(instance->uloc_color, 1, glm::value_ptr(color));
    gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
}
