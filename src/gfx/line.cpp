#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/line.hpp"
#include "gl/shader.hpp"

using Gfx::Line;

Line* Line::instance;

Line::Line()
    : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330\n"
                  "uniform vec2 pos[2];\n"
                  "void main()\n"
                  "{\n"
                  "    gl_Position = vec4(pos[gl_VertexID], 0, 1);\n"
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

    assert(uloc_pos0 != -1);
    assert(uloc_pos1 != -1);
    assert(uloc_color != -1);
}

void Line::Draw(glm::vec2 coord1, glm::vec2 coord2, glm::vec4 const& color)
{
    if (instance == nullptr) {
        instance = new Line();
    }

    instance->program.Use();
    instance->vao.Bind();
    gl33::glUniform2fv(instance->uloc_pos0, 1, glm::value_ptr(coord1));
    gl33::glUniform2fv(instance->uloc_pos1, 1, glm::value_ptr(coord2));
    gl33::glUniform4fv(instance->uloc_color, 1, glm::value_ptr(color));
    gl33::glDrawArrays(gl33::GL_LINES, 0, 2);
}
