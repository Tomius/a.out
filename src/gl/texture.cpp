#include "gl/texture.hpp"
#include "util.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_ASSERT(x) ASSERT(x)
// egyelore szerintem nem...
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#include "stb_image.h"

namespace Gl
{

Texture::Texture() : BasicManagedObject{CreateTexture()}
{
    Bind();
    // some basic setup
    gl33::glTexParameteri(gl33::GL_TEXTURE_2D, gl33::GL_TEXTURE_MIN_FILTER,
                          static_cast<gl33::GLint>(gl33::GL_LINEAR));
    gl33::glTexParameteri(gl33::GL_TEXTURE_2D, gl33::GL_TEXTURE_MAG_FILTER,
                          static_cast<gl33::GLint>(gl33::GL_LINEAR));
    gl33::glTexParameteri(gl33::GL_TEXTURE_2D, gl33::GL_TEXTURE_WRAP_S,
                          static_cast<gl33::GLint>(gl33::GL_CLAMP_TO_EDGE));
    gl33::glTexParameteri(gl33::GL_TEXTURE_2D, gl33::GL_TEXTURE_WRAP_T,
                          static_cast<gl33::GLint>(gl33::GL_CLAMP_TO_EDGE));
}

void Texture::LoadImage(const std::string& fname)
{
    int w, h, bpp;
    auto buf = stbi_load(fname.c_str(), &w, &h, &bpp, 0);
    if (!buf)
        throw ImageLoadException(ATC stbi_failure_reason()).
            AddInfo("filename", fname);
    auto tmp = AtScopeExit([=] { stbi_image_free(buf); });

    gl33::GLenum fmt, intfmt;
    switch (bpp)
    {
    case 1: intfmt = gl33::GL_R8;    fmt = gl33::GL_RED;  break;
    case 2: intfmt = gl33::GL_RG8;   fmt = gl33::GL_RG;   break;
    case 3: intfmt = gl33::GL_RGB8;  fmt = gl33::GL_RGB;  break;
    case 4: intfmt = gl33::GL_RGBA8; fmt = gl33::GL_RGBA; break;
    default: UNREACHABLE();
    }

    gl33::glPixelStorei(gl33::GL_UNPACK_ALIGNMENT, 1);
    // az internalFormat parameter tenyleg GLint, meg docs.gl szerint is...
    gl33::glTexImage2D(gl33::GL_TEXTURE_2D, 0, static_cast<gl33::GLint>(intfmt),
                       w, h, 0, fmt, gl33::GL_UNSIGNED_BYTE, buf);
}

}
