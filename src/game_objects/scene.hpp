#pragma once

#include <memory>
#include <vector>
#include "game_objects/game_object.hpp"

struct GLFWwindow;

namespace Video
{
    class Camera;
}

namespace GameObjects
{

class Scene : public GameObject
{
public:
    Scene();
    void Step(float dt) override;
    void Draw() override;

    // Callbacks
    void ScreenResized(glm::ivec2 size) override;
    void KeyAction(int key, int scancode, int action, int mods) override;
    void CharTyped(unsigned codepoint) override;
    void MouseScrolled(glm::dvec2 offset) override;
    void MouseButtonPressed(int button, int action, int mods) override;
    void MouseMoved(glm::dvec2 pos) override;

    template <typename T, typename... Args>
    T* EmplaceGameObject(Args&&... args)
    {
        T* ptr = new T{std::forward<Args>(args)...};
        ptr->scene = this;
        game_objects.emplace_back(ptr);
        return ptr;
    }

    bool RemoveGameObject(GameObject* game_object);

    Video::Camera& GetCamera() { return camera; }
    const Video::Camera& GetCamera() const { return camera; }

    GLFWwindow* GetWindow() const { return win; }
    void SetWindow(GLFWwindow* window) { win = window; }

private:
    std::vector<std::unique_ptr<GameObject>> game_objects;
    Video::Camera& camera;
    GLFWwindow* win;
};

}

