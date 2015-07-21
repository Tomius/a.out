#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/shader.hpp"
#include "gl/buffer.hpp"

#include "misc/exception.hpp"

namespace Gfx
{

class Line {
public:
    template <typename Material>
    static void Draw(glm::vec2 coord1, glm::vec2 coord2, const Material& material,
                     const glm::mat3& mvp = glm::mat3{})
    {
        LineImpl<Material>::Draw(coord1, coord2, material, mvp);
    }

private:
    template<typename Material>
    class LineImpl {
    public:
        static void Draw(glm::vec2 coord1, glm::vec2 coord2,
                         const Material& material, glm::mat3 mvp)
        {
            auto& inst = GetInstance();
            inst.program.Use();
            inst.vao.Bind();
            material.Update(inst.program);
            gl33::glUniform2fv(inst.uloc_pos0, 1, glm::value_ptr(coord1));
            gl33::glUniform2fv(inst.uloc_pos1, 1, glm::value_ptr(coord2));
            mvp[2][2] /= 2.0f; // set default depth to 0.5
            gl33::glUniformMatrix3fv(inst.uloc_mvp, 1,
                                     gl33::GL_FALSE, glm::value_ptr(mvp));
            gl33::glDrawArrays(gl33::GL_LINES, 0, 2);
        }

    private:
        Gl::Program program;
        Gl::VertexArray vao;

        gl33::GLint uloc_pos0, uloc_pos1;
        gl33::GLint uloc_mvp;

        LineImpl()  : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                              "#version 330\n"
                              "uniform vec2 pos[2];\n"
                              "uniform mat3 mvp;\n"
                              "out float texCoord;\n"
                              "void main()\n"
                              "{\n"
                              "    texCoord = gl_VertexID;\n"
                              "    vec3 p = mvp * vec3(pos[gl_VertexID], 1);"
                              "    gl_Position = vec4(p, 1);\n"
                              "}\n"
                          },
                          Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                              "#version 330\n"
                              "in float texCoord;\n"
                              "out vec4 col;\n"
                              "vec4 GetColor(vec2 texCoord);\n"
                              "void main()\n"
                              "{\n"
                              "    col = GetColor(vec2(texCoord));\n"
                              "}\n"
                          },
                          Material::GetShader()}
        {
            program.Use();
            program.BindFragDataLocation(0, "col");
            uloc_pos0 = program.GetUniformLocation("pos[0]");
            uloc_pos1 = program.GetUniformLocation("pos[1]");
            uloc_mvp = program.GetUniformLocation("mvp");

            ASSERT(uloc_pos0 != -1);
            ASSERT(uloc_pos1 != -1);
            ASSERT(uloc_mvp != -1);
        }

        static LineImpl& GetInstance()
        {
            static LineImpl line;
            return line;
        }
    };
};

}
