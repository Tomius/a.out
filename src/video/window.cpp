#include "config.h"

#include <map>
#include <iostream>
#include <stdexcept>

#include <glbinding/callbacks.h>
#include <glbinding/Binding.h>
#include <glbinding/gl33core/gl.h>
#include <glbinding/gl33ext/gl.h>

#include "logger/logger.hpp"
#include "video/window.hpp"

namespace gl33 = gl33core;

namespace Video
{

#ifdef DEBUG
static void debug_cb(gl33::GLenum source, gl33::GLenum type, gl33::GLuint id,
                     gl33::GLenum severity, gl33::GLsizei length,
                     const gl33::GLchar* message, const void*)
{
    int lvl;
    switch (severity)
    {
    case gl33ext::GL_DEBUG_SEVERITY_HIGH:
        lvl = 1;
        break;
    case gl33ext::GL_DEBUG_SEVERITY_MEDIUM:
        lvl = 2;
        break;
    case gl33ext::GL_DEBUG_SEVERITY_LOW:
        lvl = 3;
        break;
    case gl33ext::GL_DEBUG_SEVERITY_NOTIFICATION:
        lvl = 4;
        break;
    default:
        lvl = 5; // ???
        break;
    }

    DBG("opengl", lvl) << int(severity) << " " <<
        message << std::endl;
}
#endif

// Callbacks

static std::map<GLFWwindow*, GameObjects::Scene*> window2scene;

static GameObjects::Scene* GetScene(GLFWwindow* window)
{
    auto it = window2scene.find(window);
    if (it != window2scene.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

static void ScreenResizedCallback(GLFWwindow* window, int width, int height)
{
    gl33::glViewport(0, 0, width, height);
    auto scene = GetScene(window);
    if (scene)
        scene->ScreenResized(glm::ivec2{width, height});
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
        scene->MouseScrolled(glm::dvec2{xoffset, yoffset});
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
        scene->MouseMoved(glm::dvec2{xpos, ypos});
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

Window::~Window() noexcept
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
    glbinding::setUnresolvedCallback([](const glbinding::AbstractFunction& f)
    {
        ERR << "sigsegv handler " << f.name() << std::endl;
        abort();
    });

    if (Logger::mgr.IsToDebug("opengl", 1))
    {
        glbinding::setCallbackMaskExcept(
            glbinding::CallbackMask::After, { "glGetError" });
        glbinding::setAfterCallback([](const glbinding::FunctionCall& f)
        {
            gl33::GLenum error;
            while ((error = gl33::glGetError()) != gl33::GL_NO_ERROR)
                DBG("opengl", 1) << f.function->name() << " "
                                 << std::hex << error << std::endl;
        });

        gl33ext::glDebugMessageCallback(debug_cb, nullptr);
    }
#endif

    glfwSetFramebufferSizeCallback(win, ScreenResizedCallback);
    glfwSetKeyCallback(win, KeyCallback);
    glfwSetCharCallback(win, CharTypedCallback);
    glfwSetScrollCallback(win, MouseScrolledCallback);
    glfwSetMouseButtonCallback(win, MouseButtonPressedCallback);
    glfwSetCursorPosCallback(win, MouseMovedCallback);
}

GameObjects::Scene* Window::GetScene() const
{
    return Video::GetScene(win);
}

void Window::SetScene(GameObjects::Scene* scene)
{
    window2scene[win] = scene;
    scene->ScreenResized(GetSize());
}

glm::ivec2 Window::GetSize() const {
    glm::ivec2 size;
    glfwGetWindowSize(win, &size.x, &size.y);
    return size;
}

void Window::SetSize(glm::ivec2 size) {
    glfwSetWindowSize(win, size.x, size.y);
}

static void err_cb(int, const char* err)
{
    ERR << "glfw: " << err << std::endl; // todo
}

Window::GlfwInitializer::GlfwInitializer()
{
    glfwSetErrorCallback(err_cb);
    if (!glfwInit())
        throw std::runtime_error("baa baa"); // todo
}

}
