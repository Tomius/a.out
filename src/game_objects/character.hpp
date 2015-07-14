#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "random_ball.hpp"

namespace GameObjects
{

class Character : public RandomBall {
public:
    Character() : RandomBall (glm::vec2{0, 0.5f}, 0.5f,
                              glm::vec4{0.0f, 0.6f, 0.8f, 1.0f}) {}

    void Step(float dt) override {
        RandomBall::Step(dt);

        GetScene().GetCamera().viewport_center = position;

        bool in_air = position.y > radius + 0.01f;
        const float max_speed = 16;
        const float move_force = in_air ? 4 : 12;

        if (a_pressed && !d_pressed) {
            if (-velocity.x < max_speed) {
                ApplyImpulse(glm::vec2{-move_force, 0.0f} * dt);
            }
        } else if (d_pressed && !a_pressed) {
            if (velocity.x < max_speed) {
                ApplyImpulse(glm::vec2{move_force, 0.0f} * dt);
            }
        }
    }

    void KeyAction(int key, int scancode, int action, int mods) override {
        bool in_air = position.y > radius + 0.01f;

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
