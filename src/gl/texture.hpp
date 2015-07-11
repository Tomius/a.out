#pragma once

#include "gl/basic.hpp"
#include "misc/exception.hpp"

#include <glbinding/gl33core/gl.h>
namespace gl33 = gl33core;

DEF_EXCEPTION(ImageLoadException, "Failed to load image");

namespace Gl
{

inline auto CreateTexture()
{
    gl33::GLuint tex;
    gl33::glGenTextures(1, &tex);
    return tex;
}

inline void DeleteTexture(gl33::GLuint tex)
{
    gl33::glDeleteTextures(1, &tex);
}

class Texture : public BasicManagedObject<gl33::GLuint, DeleteTexture>
{
public:
    using BasicManagedObject::BasicManagedObject;
    Texture();

    void Bind(gl33::GLenum target = gl33::GL_TEXTURE_2D) const
    { glBindTexture(target, Get()); }

    void LoadImage(const std::string& fname);
};

}
