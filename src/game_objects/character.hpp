#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "gfx/circle.hpp"
#include "gfx/rectangle.hpp"
#include "gfx/material/color_material.hpp"
#include "physics/rigidbody.hpp"

namespace GameObjects
{

class Character : public GameObject, public RigidBody
{
    float radius = 0.5f;
public:
    Character() {
        inverse_mass = 1.0f;
        ApplyForce(glm::vec2{0, -9.81f});
    }

    void Draw() override {
        Gfx::Circle::Draw(position, radius, Gfx::ColorMaterial{glm::vec4(1.0f)},
                          GetScene().GetCamera().GetMatrix());
    }

    void Step(float dt) override {
        RigidBody::Step(dt);
        GetScene().GetCamera().viewport_center = position;

        bool in_air = position.y > radius;
        const float max_speed = 8;
        const float move_force = in_air ? 4 : 8;
        const float friction_force = in_air ? 1 : 4;

        if (a_pressed && !d_pressed) {
            if (-velocity.x < max_speed) {
                ApplyImpulse(glm::vec2{-move_force, 0.0f} * dt);
            }
        } else if (d_pressed && !a_pressed) {
            if (velocity.x < max_speed) {
                ApplyImpulse(glm::vec2{move_force, 0.0f} * dt);
            }
        } else {
            float speed_abs = std::abs(velocity.x);
            float speed_sign = velocity.x / speed_abs;
            if (speed_abs > 0.01f) {
                ApplyImpulse(glm::vec2{-speed_sign * friction_force, 0.0f} * dt);
            }
        }

        bool in_ground = position.y < radius;
        if (in_ground) {
            position.y = radius;
            if (velocity.y < 0) {
                velocity.y = 0;
            }
        }
    }

    void KeyAction(int key, int scancode, int action, int mods) override {
        bool in_air = position.y > radius;

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
                        ApplyImpulse({0.0f, 6.0f});
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
