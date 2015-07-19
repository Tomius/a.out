#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "math.hpp"
#include "random_ball.hpp"
#include "gfx/material/texture_material.hpp"

namespace GameObjects
{

class Character : public RandomBall {
public:
    Character() : RandomBall (glm::vec2{0, 1.0f}, 0.6f,
                              glm::vec4{0.0f, 0.6f, 0.8f, 1.0f}) {
        texMat.GetTexture().Bind();
        texMat.GetTexture().LoadImage("ball.jpg");
        texMatCross.GetTexture().Bind();
        texMatCross.GetTexture().LoadImage("crosshair.png");
    }

    void Step(float dt) override {
        RandomBall::Step(dt);

        GetScene().GetCamera().viewport_center = position;
    }

    glm::vec2 GetAcceleration(const PhysicsState& state) const override {
        bool in_air = position.y > radius + 0.01f;
        const float max_speed = 16;
        const float move_force = in_air ? 6 : 18;

        glm::vec2 accel = kGravity;

        GLFWwindow* window = GetScene().GetWindow();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            if (-velocity.x < max_speed) {
                accel.x -= move_force;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if (velocity.x < max_speed) {
                accel.x += move_force;
            }
        }

        return accel;
    }

    void KeyAction(int key, int scancode, int action, int mods) override {
        bool in_air = position.y > radius + 0.01f;

        if (action == GLFW_PRESS && key == GLFW_KEY_SPACE && !in_air) {
            ApplyImpulse({0.0f, 10.0f});
        }
    }

    void MouseMoved(glm::dvec2 pos) override {
        glm::vec2 tmp = {pos.x / 300., -pos.y / 300.};
        if( acos(glm::dot(aim, tmp) / (glm::length(aim) * glm::length(tmp))) > 2 ) {
            aim = glm::normalize(aim * 0.2f + tmp);
        }
        aim = glm::normalize(aim * 0.5f + tmp);
    }

     void Draw() override {
        Gfx::Circle::Draw(glm::vec2(), radius, texMat,
                          GetScene().GetCamera().GetMatrix() * GetMatrix());
        Gfx::Circle::Draw(position + aim * 2.f, radius / 2, texMatCross,
                          Math::MoveForward() * GetScene().GetCamera().GetMatrix());
    }

private:
    Gfx::TextureMaterial texMat, texMatCross;
    glm::vec2 aim = {1, 0};
};

}
