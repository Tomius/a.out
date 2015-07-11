#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "gfx/pixel.hpp"
#include "gl/shader.hpp"
#include "misc/exception.hpp"

namespace Gfx
{

template<typename Material>
Pixel::PixelImpl<Material>::PixelImpl()
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

template<typename Material>
void Pixel::PixelImpl<Material>::Draw(glm::vec2 coord, const Material& material,
                                      glm::mat3 const& mvp)
{
    GetInstance().program.Use();
    GetInstance().vao.Bind();
    material.UploadUniforms(GetInstance().program);
    gl33::glUniform2fv(GetInstance().uloc_pos, 1, glm::value_ptr(coord));
    gl33::glUniformMatrix3fv(GetInstance().uloc_mvp, 1,
                             gl33::GL_FALSE, glm::value_ptr(mvp));
    gl33::glDrawArrays(gl33::GL_POINTS, 0, 1);
}

template<typename Material>
Pixel::PixelImpl<Material>& Pixel::PixelImpl<Material>::GetInstance()
{
    static PixelImpl<Material> pixel;
    return pixel;
}

}
