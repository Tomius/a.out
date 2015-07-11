#pragma once

#include "gl/shader.hpp"
#include "gl/texture.hpp"

namespace Gfx
{

class TextureMaterial
{
public:
    TextureMaterial(Gl::Texture&& tex) : tex{std::move(tex)} {}

    static const Gl::Shader& GetShader()
    {
        static Gl::Shader shader{gl33::GL_FRAGMENT_SHADER,
            "#version 330\n"
            "uniform sampler2D TextureMaterial_tex;\n"
            "vec4 GetColor(vec2 texcoord)\n"
            "{\n"
            "    return texture2D(TextureMaterial_tex, texcoord);\n"
            "}\n"
        };
        return shader;
    }

    void UploadUniforms(const Gl::Program& program) const {
        auto uloc_tex = program.GetUniformLocation("TextureMaterial_tex");
        ASSERT(uloc_tex != -1);
        tex.Bind();
        gl33::glUniform1i(uloc_tex, 0);
    }

private:
    Gl::Texture tex;
};

}
