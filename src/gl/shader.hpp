#pragma once

#include "gl/basic.hpp"

#include <glbinding/gl33core/gl.h>
namespace gl33 = gl33core;

namespace Gl
{

class Shader : public BasicManagedObject<gl33::GLuint, gl33::glDeleteShader>
{
public:
    using BasicManagedObject::BasicManagedObject;
    Shader(gl33::GLenum shader_type) :
        BasicManagedObject{gl33::glCreateShader(shader_type)} {}
    Shader(gl33::GLenum shader_type, const char* str)
        : Shader{shader_type}
    { Compile(str); }

    void Compile(const char* str);
};

class Program : public BasicManagedObject<gl33::GLuint, gl33::glDeleteProgram>
{
public:
    using BasicManagedObject::BasicManagedObject;

    Program() : BasicManagedObject{gl33::glCreateProgram()} {}

    template <typename... Args>
    Program(Args&&... args) : Program{}
    {
        Link(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Link(Args&&... args)
    {
        Attach(std::forward<Args>(args)...);
        DoLink();
    }

    void Use() { gl33::glUseProgram(Get()); }
    auto GetAttribLocation(const char* attr)
    { return gl33::glGetAttribLocation(Get(), attr); }

    void BindFragDataLocation(gl33::GLuint color, const char* name)
    { gl33::glBindFragDataLocation(Get(), color, name); }

    auto GetUniformLocation(const char* uniform) {
        return gl33::glGetUniformLocation(Get(), uniform);
    }
private:
    template <typename... Rest>
    void Attach(const Shader& sh, Rest&&... rest)
    {
        gl33::glAttachShader(Get(), sh.Get());
        Attach(std::forward<Rest>(rest)...);
    }
    void Attach() {}
    void DoLink();
};

}
