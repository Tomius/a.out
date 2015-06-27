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
};

inline GameObject::~GameObject() = default;

}

#endif
