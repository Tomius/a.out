#ifndef UUID_998D4729_71F3_4339_8342_0401C8997F12
#define UUID_998D4729_71F3_4339_8342_0401C8997F12
#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Video
{

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window() noexcept { glfwDestroyWindow(win); }

    bool IsClosing() noexcept { return glfwWindowShouldClose(win); }
    void SetClosing(bool c) noexcept { glfwSetWindowShouldClose(win, c); }

    void SwapBuffers() noexcept { glfwSwapBuffers(win); glfwPollEvents(); }
private:
    GLFWwindow* win;

    struct GlfwInitializer
    {
        GlfwInitializer();
        ~GlfwInitializer() noexcept { glfwTerminate(); }
    };
    static void EnsureGlfw() { static GlfwInitializer init; }
};

}

#endif
