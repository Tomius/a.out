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

class Pixel {
public:
    template <typename Material>
    static void Draw(glm::vec2 coord, const Material& material,
                     const glm::mat3& mvp = glm::mat3{}) {
        PixelImpl<Material>::Draw(coord, material, mvp);
    }

private:
    template<typename Material>
    class PixelImpl {
    public:
        static void Draw(glm::vec2 coord, const Material& material,
                         glm::mat3 mvp)
        {
            auto& inst = GetInstance();
            inst.program.Use();
            inst.vao.Bind();
            material.Update(inst.program);
            gl33::glUniform2fv(inst.uloc_pos, 1, glm::value_ptr(coord));
            mvp[2][2] /= 2.0f; // set default depth to 0.5
            gl33::glUniformMatrix3fv(inst.uloc_mvp, 1,
                                     gl33::GL_FALSE, glm::value_ptr(mvp));
            gl33::glDrawArrays(gl33::GL_POINTS, 0, 1);
        }

    private:
        Gl::Program program;
        Gl::VertexArray vao;

        gl33::GLint uloc_pos;
        gl33::GLint uloc_mvp;

        PixelImpl()
            : program{Gl::Shader{gl33::GL_VERTEX_SHADER,
                          "#version 330\n"
                          "uniform vec2 pos;\n"
                          "uniform mat3 mvp;\n"
                          "void main()\n"
                          "{\n"
                          "    gl_Position = vec4(mvp * vec3(pos, 1), 1);\n"
                          "}\n"
                      },
                      Gl::Shader{gl33::GL_FRAGMENT_SHADER,
                          "#version 330\n"
                          "out vec4 col;\n"
                          "vec4 GetColor(vec2 texCoord);\n"
                          "void main()\n"
                          "{\n"
                          "    col = GetColor(vec2(0.0));\n"
                          "}\n"
                      },
                      Material::GetShader()}
        {
            program.Use();
            program.BindFragDataLocation(0, "col");
            uloc_pos = program.GetUniformLocation("pos");
            uloc_mvp = program.GetUniformLocation("mvp");

            ASSERT(uloc_pos != -1);
            ASSERT(uloc_mvp != -1);
        }

        static PixelImpl& GetInstance()
        {
            static PixelImpl<Material> pixel;
            return pixel;
        }
    };

};

}

// #include "pixel_impl.hpp"
