#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "random_ball.hpp"

namespace GameObjects
{

class Character : public RandomBall {
public:
    Character() : RandomBall (glm::vec2{0, 1.0f}, 0.6f,
                              glm::vec4{0.0f, 0.6f, 0.8f, 1.0f}) {}

    void Step(float dt) override {
        RandomBall::Step(dt);

        GetScene().GetCamera().viewport_center = position;

        bool in_air = position.y > radius + 0.01f;
        const float max_speed = 16;
        const float move_force = in_air ? 2 : 6;

        GLFWwindow* window = GetScene().GetWindow();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            if (-velocity.x < max_speed) {
                ApplyImpulse(glm::vec2{-move_force, 0.0f} * dt);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if (velocity.x < max_speed) {
                ApplyImpulse(glm::vec2{move_force, 0.0f} * dt);
            }
        }
    }

    void KeyAction(int key, int scancode, int action, int mods) override {
        bool in_air = position.y > radius + 0.01f;

        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE && !in_air) {
            ApplyImpulse({0.0f, 3.0f});
        }
    }
};

}
