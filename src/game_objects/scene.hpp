#ifndef UUID_7C0D7D7E_1D08_11E5_9A21_1697F925EC7B
#define UUID_7C0D7D7E_1D08_11E5_9A21_1697F925EC7B
#pragma once

#include <memory>
#include <vector>
#include "game_objects/game_object.hpp"

namespace GameObjects
{

class Scene : GameObject
{
public:
    virtual void Step(double dt) override;
    virtual void Draw() override;

    // Callbacks
    virtual void ScreenResized(int width, int height) override;
    virtual void KeyAction(int key, int scancode, int action, int mods) override;
    virtual void CharTyped(unsigned codepoint) override;
    virtual void MouseScrolled(double xoffset, double yoffset) override;
    virtual void MouseButtonPressed(int button, int action, int mods) override;
    virtual void MouseMoved(double xpos, double ypos) override;

    template <typename T, typename... Args>
    T* EmplaceGameObject(Args&&... args)
    {
        T* ptr = new T{std::forward<Args>(args)...};
        game_objects.push_back (std::unique_ptr<T>{ptr});
        return ptr;
    }

    bool RemoveGameObject(GameObject* game_object);

private:
    std::vector<std::unique_ptr<GameObject>> game_objects;
};

}

#endif

