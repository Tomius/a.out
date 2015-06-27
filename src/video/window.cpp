#include "config.h"

#include <iostream>
#include <stdexcept>

#include <glbinding/callbacks.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/gl33core.h>

#include "video/window.hpp"

namespace gl33 = gl33core;

namespace Video
{

Window::Window(int width, int height, const char* title)
{
    EnsureGlfw();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    win = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!win)
        throw std::runtime_error("boo boo"); // todo
    glfwMakeContextCurrent(win);

    glbinding::Binding::initialize();
#ifdef DEBUG
    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });
    glbinding::setAfterCallback([](const glbinding::FunctionCall& f)
    {
        gl33::GLenum error;
        while ((error = gl33::glGetError()) != gl33::GL_NO_ERROR)
            std::cerr << "error: " << f.toString() << " " // todo
                      << std::hex << error << std::endl;
    });

    glbinding::setUnresolvedCallback([](const glbinding::AbstractFunction& f)
    {
        std::cerr << "sigsegv handler " << f.name() << std::endl; // todo
        abort();
    });
#endif
}

static void err_cb(int, const char* err)
{
    std::cerr << "glfw: " << err << std::endl;
}

Window::GlfwInitializer::GlfwInitializer()
{
    glfwSetErrorCallback(err_cb);
    if (!glfwInit())
        throw std::runtime_error("baa baa"); // todo
}

}
