#include "gfx/simple_bitmap_font.hpp"

namespace Gfx
{

SimpleBitmapFont::SimpleBitmapFont(const std::string& filename, float width,
                                   float height)
    : mat{{0,0, 1/16.f, 1/6.f}}, width{width}, height{height}
{
    mat.GetTexture().LoadImage(filename);
    gl33::glTexParameteri(gl33::GL_TEXTURE_2D, gl33::GL_TEXTURE_MAG_FILTER,
                          static_cast<gl33::GLint>(gl33::GL_NEAREST));
}

void SimpleBitmapFont::Draw(const std::string& str, const glm::vec2& pos,
                            const glm::mat3& mvp) const
{
    Rect<float> dst = {pos.x, pos.y, width, height};

    for (unsigned char c : str)
    {
        c -= 32;
        if (c > 95) c = 95;

        mat.GetRect().y = (c/16) / 6.f;
        mat.GetRect().x = (c%16) / 16.f;

        Rectangle::Draw(dst, mat, mvp);
        dst.x += width;
    }
}

}
