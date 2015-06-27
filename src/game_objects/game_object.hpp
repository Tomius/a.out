#ifndef UUID_A237F83A_8EA9_4AD9_AB4A_895BB85DF37E
#define UUID_A237F83A_8EA9_4AD9_AB4A_895BB85DF37E
#pragma once

namespace GameObjects
{

class GameObject
{
public:
    virtual ~GameObject() = 0;

    virtual void Step(double dt) {}
    virtual void Draw() {}

    // Callbacks
    virtual void ScreenResized(int width, int height) {}
    virtual void KeyAction(int key, int scancode, int action, int mods) {}
    virtual void CharTyped(unsigned codepoint) {}
    virtual void MouseScrolled(double xoffset, double yoffset) {}
    virtual void MouseButtonPressed(int button, int action, int mods) {}
    virtual void MouseMoved(double xpos, double ypos) {}
};

inline GameObject::~GameObject() = default;

}

#endif
