#pragma once

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rect.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"

namespace Gfx
{

class Rectangle {
public:
    template <typename Material>
    static void Draw(Rect<float> rect, const Material& material,
                     const glm::mat3& mvp = glm::mat3{})
    {
        RectangleImpl<Material>::Draw(rect, material, mvp);
    }

private:
    template <typename Material>
    class RectangleImpl
    {
    public:
        static void Draw(Rect<float> rect, const Material& material,
                         const glm::mat3& mvp)
        {
            auto& inst = GetInstance();
            inst.program.Use();
            inst.vao.Bind();
            material.Update(inst.program);
            gl33::glUniform2fv(inst.uloc_bottom_left, 1, &rect.x);
            gl33::glUniform2fv(inst.uloc_size, 1, &rect.width);
            gl33::glUniformMatrix3fv(inst.uloc_mvp, 1,
                                     gl33::GL_FALSE, glm::value_ptr(mvp));
            gl33::glDrawArrays(gl33::GL_TRIANGLE_FAN, 0, 4);
        }
    private:
        Gl::Program program;
        Gl::VertexArray vao;

        gl33::GLint uloc_bottom_left;
        gl33::GLint uloc_size;
        gl33::GLint uloc_mvp;

        RectangleImpl()
            : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                  "#version 330 core\n"
                  "const vec2 position[4] = vec2[](\n"
                  "    vec2(0, 0),\n"
                  "    vec2(0, 1),\n"
                  "    vec2(1, 1),\n"
                  "    vec2(1, 0)\n"
                  ");\n"
                  "uniform vec2 bottom_left;\n"
                  "uniform vec2 size;\n"
                  "uniform mat3 mvp;\n"
                  "out vec2 texcoord;\n"
                  "void main()\n"
                  "{\n"
                  "    vec2 pos = position[gl_VertexID];\n"
                  "    texcoord = vec2(pos.x, 1 - pos.y);\n"
                  "    pos = bottom_left + size*pos;\n"
                  "    gl_Position = vec4((mvp*vec3(pos,1)).xy, 0, 1);\n"
                  "}\n"
              },
              Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                  "#version 330 core\n"
                  "in vec2 texcoord;\n"
                  "out vec4 col;\n"
                  "vec4 GetColor(vec2 texcoord);\n"
                  "void main()\n"
                  "{\n"
                  "    col = GetColor(texcoord);\n"
                  "}\n"
              },
              Material::GetShader()}
        {
            program.Use();
            program.BindFragDataLocation(0, "col");
            uloc_bottom_left = program.GetUniformLocation("bottom_left");
            uloc_size = program.GetUniformLocation("size");
            uloc_mvp = program.GetUniformLocation("mvp");

            ASSERT(uloc_bottom_left != -1);
            ASSERT(uloc_size != -1);
            ASSERT(uloc_mvp != -1);
        }


        static RectangleImpl& GetInstance()
        {
            static RectangleImpl rect;
            return rect;
        }
    };
};

}
