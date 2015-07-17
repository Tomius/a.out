#pragma once

#include "gl/shader.hpp"
#include "gl/texture.hpp"

namespace Gfx
{

class TextureMaterial
{
public:
    TextureMaterial() {}
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

    void Update(const Gl::Program& program) const {
        tex.Bind();
    }

    Gl::Texture& GetTexture() { return tex; }

private:
    Gl::Texture tex;
};

}
