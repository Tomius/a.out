#pragma once

#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "gfx/circle.hpp"
#include "gfx/rectangle.hpp"
#include "physics/physics_state.hpp"

namespace GameObjects
{

class Character : public GameObject
{
public:
    Physics::PhysicsState state;

    Character() {
        state.ApplyForceField(Physics::kGravity);
    }

    void Draw() override {
        glm::vec2 pos(state.pos + glm::dvec2(0, 0.5));
        Gfx::Circle::Draw(pos, 0.5f, glm::vec4(1.0f),
                          GetScene().GetCamera().GetMatrix());
    }

    void Step(double dt) override {
        state.Step(dt);
        GetScene().GetCamera().viewport_center = state.pos;

        bool in_air = state.pos.y != 0;
        const float max_speed = 8;
        const float move_force = in_air ? 4 : 8;
        const float friction_force = in_air ? 0 : 4;

        if (a_pressed && !d_pressed) {
            if (-state.speed.x < max_speed) {
                state.ApplyForce({-move_force, 0.0f});
            }
        } else if (d_pressed && !a_pressed) {
            if (state.speed.x < max_speed) {
                state.ApplyForce({move_force, 0.0f});
            }
        } else {
            float speed_abs = std::abs(state.speed.x);
            float speed_sign = state.speed.x / speed_abs;
            if (speed_abs > 0.01f) {
                state.ApplyForce({-speed_sign * friction_force, 0.0f});
            }
        }
    }

    void KeyAction(int key, int scancode, int action, int mods) override {
        bool in_air = state.pos.y != 0;

        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_A:
                    a_pressed = true;
                    break;
                case GLFW_KEY_D:
                    d_pressed = true;
                    break;
                case GLFW_KEY_SPACE:
                    if (!in_air) {
                        state.ApplyImpulse({0.0f, 6.0f});
                    }
                    break;
            }
        } else if (action == GLFW_RELEASE) {
            switch (key) {
                case GLFW_KEY_A:
                    a_pressed = false;
                    break;
                case GLFW_KEY_D:
                    d_pressed = false;
                    break;
            }
        }
    }

private:
    bool a_pressed = false, d_pressed = false;
};

}
