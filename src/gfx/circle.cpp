#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/circle.hpp"
#include "gl/shader.hpp"
#include "misc/exception.hpp"

using Gfx::Circle;

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
                  "uniform mat3 mvp;\n"
                  "out vec2 pos;\n"
                  "void main()\n"
                  "{\n"
                  "    pos = center + radius*data[gl_VertexID];\n"
                  "    gl_Position = vec4((mvp*vec3(pos,1)).xy, 0, 1);\n"
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
    uloc_mvp = program.GetUniformLocation("mvp");

    ASSERT(uloc_center != -1);
    ASSERT(uloc_radius != -1);
    ASSERT(uloc_color != -1);
    ASSERT(uloc_mvp != -1);
}

void Circle::Draw(glm::vec2 center, float radius, glm::vec4 const& color,
                  glm::mat3 const& mvp)
{
    GetInstance().program.Use();
    GetInstance().vao.Bind();
    gl33::glUniform2fv(GetInstance().uloc_center, 1, glm::value_ptr(center));
    gl33::glUniform1f(GetInstance().uloc_radius, radius);
    gl33::glUniform4fv(GetInstance().uloc_color, 1, glm::value_ptr(color));
    gl33::glUniformMatrix3fv(GetInstance().uloc_mvp, 1,
                             gl33::GL_FALSE, glm::value_ptr(mvp));
    gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
}

Circle& Circle::GetInstance()
{
    static Circle circle;
    return circle;
}
