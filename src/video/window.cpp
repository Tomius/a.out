#include "config.h"

#include <map>
#include <iostream>
#include <stdexcept>

#include <glbinding/callbacks.h>
#include <glbinding/Binding.h>
#include <glbinding/gl33core/gl.h>
#include <glbinding/gl33ext/gl.h>

#include "video/window.hpp"

namespace gl33 = gl33core;

namespace Video
{

#ifdef DEBUG
static void debug_cb(gl33::GLenum source, gl33::GLenum type, gl33::GLuint id,
                     gl33::GLenum severity, gl33::GLsizei length,
                     const gl33::GLchar* message, const void*)
{
    std::cerr << message << std::endl;
}
#endif

// Callbacks

static std::map<GLFWwindow*, GameObjects::Scene*> window2scene;

static GameObjects::Scene* GetScene(GLFWwindow* window)
{
    auto it = window2scene.find(window);
    if (it != window2scene.end())
    {
        return it->second;
    } else {
        return nullptr;
    }
}

static void ScreenResizedCallback(GLFWwindow* window, int width, int height)
{
    auto scene = GetScene(window);
    if (scene)
        scene->ScreenResized(width, height);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode,
                        int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) // todo
        glfwSetWindowShouldClose(window, true);

    auto scene = GetScene(window);
    if (scene)
        scene->KeyAction(key, scancode, action, mods);
}

static void CharTypedCallback(GLFWwindow* window, unsigned codepoint)
{
    auto scene = GetScene(window);
    if (scene)
        scene->CharTyped(codepoint);
}

static void MouseScrolledCallback(GLFWwindow* window, double xoffset,
                                  double yoffset)
{
    auto scene = GetScene(window);
    if (scene)
        scene->MouseScrolled(xoffset, yoffset);
}

static void MouseButtonPressedCallback(GLFWwindow* window, int button,
                                       int action, int mods)
{
    auto scene = GetScene(window);
    if (scene)
        scene->MouseButtonPressed(button, action, mods);
}

static void MouseMovedCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto scene = GetScene(window);
    if (scene)
        scene->MouseMoved(xpos, ypos);
}

// Ctors

Window::Window(const char* title)
{
  EnsureGlfw();
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

  Init(vidmode->width, vidmode->height, monitor, title);
}

Window::Window(int width, int height, const char* title)
{
  EnsureGlfw();
  Init(width, height, nullptr, title);
}

Window::~Window()
{
    window2scene.erase(win);
    glfwDestroyWindow(win);
}

void Window::Init(int width, int height, GLFWmonitor* monitor, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    win = glfwCreateWindow(width, height, title, monitor, nullptr);
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
            std::cerr << "error: " << f.function->name() << " " // todo
                      << std::hex << error << std::endl;
    });

    glbinding::setUnresolvedCallback([](const glbinding::AbstractFunction& f)
    {
        std::cerr << "sigsegv handler " << f.name() << std::endl; // todo
        abort();
    });

    gl33ext::glDebugMessageCallback(debug_cb, nullptr);
#endif

    glfwSetFramebufferSizeCallback(win, ScreenResizedCallback);
    glfwSetKeyCallback(win, KeyCallback);
    glfwSetCharCallback(win, CharTypedCallback);
    glfwSetScrollCallback(win, MouseScrolledCallback);
    glfwSetMouseButtonCallback(win, MouseButtonPressedCallback);
    glfwSetCursorPosCallback(win, MouseMovedCallback);
}

void Window::SetScene(GameObjects::Scene* scene)
{
    window2scene[win] = scene;
}

GameObjects::Scene* Window::GetScene() const
{
    return Video::GetScene(win);
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
