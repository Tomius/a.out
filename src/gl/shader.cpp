#include <iostream>
#include "util.hpp"
#include "gl/shader.hpp"

namespace Gl
{

void Shader::Compile(const char *str)
{
    gl33::glShaderSource(Get(), 1, &str, nullptr);
    gl33::glCompileShader(Get());

    gl33::GLint stat;
    gl33::glGetShaderiv(Get(), gl33::GL_COMPILE_STATUS, &stat);
    if (!stat)
    {
        gl33::glGetShaderiv(Get(), gl33::GL_INFO_LOG_LENGTH, &stat);
        auto buf = make_unique<char>(stat);
        gl33::glGetShaderInfoLog(Get(), stat, nullptr, buf.get());
        std::cerr << buf.get() << std::endl; // todo
        throw std::runtime_error("baa baa"); // todo
    }
}

void Program::DoLink()
{
    gl33::glLinkProgram(Get());

    gl33::GLint stat;
    gl33::glGetProgramiv(Get(), gl33::GL_LINK_STATUS, &stat);
    if (!stat)
    {
        gl33::glGetProgramiv(Get(), gl33::GL_INFO_LOG_LENGTH, &stat);
        auto buf = make_unique<char>(stat);
        gl33::glGetProgramInfoLog(Get(), stat, nullptr, buf.get());
        std::cerr << buf.get() << std::endl; // todo
        throw std::runtime_error("baa baa"); // todo
    }

}

}
