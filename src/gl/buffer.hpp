#pragma once

#include "gl/basic.hpp"

#include <glbinding/gl33core/gl.h>
namespace gl33 = gl33core;

namespace Gl
{

inline auto CreateBuffer()
{
    gl33::GLuint buf;
    gl33::glGenBuffers(1, &buf);
    return buf;
}

inline void DeleteBuffer(gl33::GLuint buf)
{
    gl33::glDeleteBuffers(1, &buf);
}

class Buffer : public BasicManagedObject<gl33::GLuint, DeleteBuffer>
{
public:
    using BasicManagedObject::BasicManagedObject;

    Buffer() : BasicManagedObject(CreateBuffer()) {}
    void Bind(gl33::GLenum target) { gl33::glBindBuffer(target, Get()); }
};

inline auto CreateVertexArray()
{
    gl33::GLuint buf;
    gl33::glGenVertexArrays(1, &buf);
    return buf;
}

inline void DeleteVertexArray(gl33::GLuint buf)
{
    gl33::glDeleteVertexArrays(1, &buf);
}

class VertexArray : public BasicManagedObject<gl33::GLuint, DeleteVertexArray>
{
public:
    using BasicManagedObject::BasicManagedObject;

    VertexArray() : BasicManagedObject(CreateVertexArray()) {}
    void Bind() { gl33::glBindVertexArray(Get()); }
};

}
