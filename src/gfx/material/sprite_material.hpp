#pragma once

#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "rect.hpp"

namespace Gfx
{

class SpriteMaterial
{
public:
    SpriteMaterial(Rect<float> rect)
        : rect(rect) {}
    SpriteMaterial(Gl::Texture&& tex, Rect<float> rect)
        : tex{std::move(tex)}, rect(rect) {}

    static const Gl::Shader& GetShader()
    {
        static Gl::Shader shader{gl33::GL_FRAGMENT_SHADER,
            "#version 330\n"
            "uniform sampler2D SpriteMaterial_tex;\n"
            "uniform vec2 SpriteMaterial_scale;\n"
            "uniform vec2 SpriteMaterial_offs;\n"
            "vec4 GetColor(vec2 texcoord)\n"
            "{\n"
            "    vec2 coord = texcoord * SpriteMaterial_scale + \n"
            "        SpriteMaterial_offs;\n"
            "    return texture2D(SpriteMaterial_tex, coord);\n"
            "}\n"
        };
        return shader;
    }

    void Update(const Gl::Program& program) const
    {
        tex.Bind();

        gl33::GLint uloc_offs = program.GetUniformLocation("SpriteMaterial_offs");
        gl33::GLint uloc_scale = program.GetUniformLocation("SpriteMaterial_scale");
        ASSERT(uloc_offs != -1);
        ASSERT(uloc_scale != -1);

        gl33::glUniform2f(uloc_offs, rect.x, rect.y);
        gl33::glUniform2f(uloc_scale, rect.width, rect.height);
    }

    Gl::Texture& GetTexture() { return tex; }

    Rect<float>& GetRect() { return rect; }
private:
    Gl::Texture tex;
    Rect<float> rect;
};

}
