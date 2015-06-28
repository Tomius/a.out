#ifndef UUID_998D4729_71F3_4339_8342_0401C8997F12
#define UUID_998D4729_71F3_4339_8342_0401C8997F12
#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "game_objects/scene.hpp"

namespace Video
{

class Window
{
public:
    Window(const char* title); // full screen
    Window(int width, int height, const char* title);
    ~Window() noexcept;

    bool IsClosing() const noexcept { return glfwWindowShouldClose(win); }
    void SetClosing(bool c) noexcept { glfwSetWindowShouldClose(win, c); }

    void SwapBuffers() noexcept { glfwSwapBuffers(win); glfwPollEvents(); }

    void SetScene(GameObjects::Scene* scene);
    GameObjects::Scene* GetScene() const;

private:
    GLFWwindow* win;

    struct GlfwInitializer
    {
        GlfwInitializer();
        ~GlfwInitializer() noexcept { glfwTerminate(); }
    };
    static void EnsureGlfw() { static GlfwInitializer init; }

    void Init(int width, int height, GLFWmonitor* monitor, const char* title);
};

}

#endif
