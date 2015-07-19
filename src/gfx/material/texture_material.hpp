#pragma once

#include "gl/shader.hpp"
#include "gl/texture.hpp"

namespace Gfx
{

class TextureMaterial
{
public:
    TextureMaterial(float alpha = 1) : alpha{alpha} {}
    TextureMaterial(Gl::Texture&& tex, float alpha = 1)
        : tex{std::move(tex)}, alpha{alpha} {}

    static const Gl::Shader& GetShader()
    {
        static Gl::Shader shader{gl33::GL_FRAGMENT_SHADER,
            "#version 330\n"
            "uniform sampler2D TextureMaterial_tex;\n"
            "uniform float TextureMaterial_alpha;\n"
            "vec4 GetColor(vec2 texcoord)\n"
            "{\n"
            "    vec4 col = texture2D(TextureMaterial_tex, texcoord);\n"
            "    return vec4(col.rgb, TextureMaterial_alpha);\n"
            "}\n"
        };
        return shader;
    }

    void Update(const Gl::Program& program) const {
        tex.Bind();

        gl33::GLint uloc_alpha = program.GetUniformLocation("TextureMaterial_alpha");
        ASSERT(uloc_alpha != -1);
        gl33::glUniform1f(uloc_alpha, alpha);
    }

    Gl::Texture& GetTexture() { return tex; }

private:
    Gl::Texture tex;
    float alpha;
};

}
