#pragma once

#include <glm/vec2.hpp>

namespace GameObjects
{

class Scene;

class GameObject
{
public:
    virtual ~GameObject() = 0;

    virtual void Step(double dt) {}
    virtual void Draw() {}

    // Callbacks
    virtual void ScreenResized(glm::ivec2 size) {}
    virtual void KeyAction(int key, int scancode, int action, int mods) {}
    virtual void CharTyped(unsigned codepoint) {}
    virtual void MouseScrolled(glm::dvec2 offset) {}
    virtual void MouseButtonPressed(int button, int action, int mods) {}
    virtual void MouseMoved(glm::dvec2 pos) {}

    Scene& GetScene() { return *scene; }
    Scene const& GetScene() const { return *scene; }

private:
    friend class Scene;
    Scene *scene;
};

inline GameObject::~GameObject() = default;

}
