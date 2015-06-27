#include <iostream>
#include "util.hpp"
#include "gl/shader.hpp"

namespace Gl
{

void Shader::Compile(const char *str)
{
    gl33::glShaderSource(Get(), 1, &str, nullptr);
    gl33::glCompileShader(Get());

    gl33::GLint length;
    gl33::glGetShaderiv(Get(), gl33::GL_INFO_LOG_LENGTH, &length);
    if (length > 1) // even an empty message consist of a new line character
    {
        auto buf = make_unique<char[]>(length);
        gl33::glGetShaderInfoLog(Get(), length, nullptr, buf.get());
        std::cerr << buf.get() << std::endl; // todo

        gl33::GLint success;
        gl33::glGetShaderiv(Get(), gl33::GL_COMPILE_STATUS, &success);
        if (!success)
        {
          throw std::runtime_error("GLSL compile error"); // todo
        }
    }

}

void Program::DoLink()
{
    gl33::glLinkProgram(Get());

    gl33::GLint length;
    gl33::glGetProgramiv(Get(), gl33::GL_INFO_LOG_LENGTH, &length);
    if (length > 1) // even an empty message consist of a new line character
    {
        auto buf = make_unique<char[]>(length);
        gl33::glGetProgramInfoLog(Get(), length, nullptr, buf.get());
        std::cerr << buf.get() << std::endl; // todo

        gl33::GLint success;
        gl33::glGetProgramiv(Get(), gl33::GL_LINK_STATUS, &success);
        if (!success)
        {
          throw std::runtime_error("GLSL link error"); // todo
        }
    }
}

}
