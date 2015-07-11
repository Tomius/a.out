#pragma once

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"

#include "misc/exception.hpp"

namespace Gfx
{

class Circle {
public:
    template<typename Material>
    static void Draw(glm::vec2 center, float radius, const Material& material,
                     const glm::mat3& mvp = glm::mat3{})
    {
        CircleImpl<Material>::Draw(center, radius, material, mvp);
    }

private:
    template<typename Material>
    class CircleImpl {
    public:
        static void Draw(glm::vec2 center, float radius,
                         const Material& material, const glm::mat3& mvp)
        {
            auto& inst = GetInstance();
            inst.program.Use();
            inst.vao.Bind();
            material.Update(inst.program);
            gl33::glUniform2fv(inst.uloc_center, 1, glm::value_ptr(center));
            gl33::glUniform1f(inst.uloc_radius, radius);
            gl33::glUniformMatrix3fv(inst.uloc_mvp, 1,
                                     gl33::GL_FALSE, glm::value_ptr(mvp));
            gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
        }

    private:
        Gl::Program program;
        Gl::VertexArray vao;

        gl33::GLint uloc_center;
        gl33::GLint uloc_radius;
        gl33::GLint uloc_mvp;

        CircleImpl() : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
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
                      "out vec2 texCoord;"
                      "void main()\n"
                      "{\n"
                      "    vec2 position = data[gl_VertexID];\n"
                      "    texCoord = (vec2(position.x, -position.y)+1)/2;\n"
                      "    pos = center + radius*position;\n"
                      "    gl_Position = vec4((mvp*vec3(pos,1)).xy, 0, 1);\n"
                      "}\n"
                  },
                  Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                      "#version 330 core\n"
                      "in vec2 pos;\n"
                      "in vec2 texCoord;\n"
                      "out vec4 col;\n"
                      "vec4 GetColor(vec2 texCoord);\n"
                      "uniform vec2 center;\n"
                      "uniform float radius;\n"
                      "void main()\n"
                      "{\n"
                      "    if (length(pos - center) > radius) discard;\n"
                      "    col = GetColor(texCoord);\n"
                      "}"
                  },
                  Material::GetShader()}
        {
            program.Use();
            program.BindFragDataLocation(0, "col");
            uloc_center = program.GetUniformLocation("center");
            uloc_radius = program.GetUniformLocation("radius");
            uloc_mvp = program.GetUniformLocation("mvp");

            ASSERT(uloc_center != -1);
            ASSERT(uloc_radius != -1);
            ASSERT(uloc_mvp != -1);
        }

        static CircleImpl& GetInstance()
        {
            static CircleImpl circle;
            return circle;
        }
    };
};

}
