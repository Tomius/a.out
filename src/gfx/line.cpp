#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/line.hpp"
#include "gl/shader.hpp"
#include "misc/exception.hpp"

using Gfx::Line;

Line::Line()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330\n"
                  "uniform vec2 pos[2];\n"
                  "uniform mat3 mvp;\n"
                  "void main()\n"
                  "{\n"
                  "    vec2 p = (mvp*vec3(pos[gl_VertexID],1)).xy;"
                  "    gl_Position = vec4(p, 0, 1);\n"
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
    uloc_pos0 = program.GetUniformLocation("pos[0]");
    uloc_pos1 = program.GetUniformLocation("pos[1]");
    uloc_color = program.GetUniformLocation("color");
    uloc_mvp = program.GetUniformLocation("mvp");

    ASSERT(uloc_pos0 != -1);
    ASSERT(uloc_pos1 != -1);
    ASSERT(uloc_color != -1);
    ASSERT(uloc_mvp != -1);
}

void Line::Draw(glm::vec2 coord1, glm::vec2 coord2,
                glm::vec4 const& color, glm::mat3 const& mvp)
{
    GetInstance().program.Use();
    GetInstance().vao.Bind();
    gl33::glUniform2fv(GetInstance().uloc_pos0, 1, glm::value_ptr(coord1));
    gl33::glUniform2fv(GetInstance().uloc_pos1, 1, glm::value_ptr(coord2));
    gl33::glUniform4fv(GetInstance().uloc_color, 1, glm::value_ptr(color));
    gl33::glUniformMatrix3fv(GetInstance().uloc_mvp, 1,
                             gl33::GL_FALSE, glm::value_ptr(mvp));
    gl33::glDrawArrays(gl33::GL_LINES, 0, 2);
}

Line& Line::GetInstance()
{
    static Line line;
    return line;
}
