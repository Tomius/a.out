#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

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
    
    void HideCursor();
    void ShowCursor();
    bool GetCursorState();

    void SwapBuffers() noexcept { glfwSwapBuffers(win); glfwPollEvents(); }

    GameObjects::Scene* GetScene() const;
    void SetScene(GameObjects::Scene* scene);

    glm::ivec2 GetSize() const;
    void SetSize(glm::ivec2 size);

private:
    GLFWwindow* win;

    struct GlfwInitializer
    {
        GlfwInitializer();
        ~GlfwInitializer() noexcept { glfwTerminate(); }
    };
    static void EnsureGlfw() { static GlfwInitializer init; }

    void Init(int width, int height, GLFWmonitor* monitor, const char* title);

    bool CursorEnabled = true;
};

}
