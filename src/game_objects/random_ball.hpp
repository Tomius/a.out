#pragma once

#include "gfx/circle.hpp"
#include "gfx/material/color_material.hpp"
#include "physics/rigidbody.hpp"

namespace GameObjects {

class RandomBall : public GameObject, public RigidBody {
public:
    RandomBall(glm::vec2 initial_pos, float radius,
               glm::vec4 color = glm::vec4(1.0f))
            : radius(radius), color(color) {
        inverse_mass = 1 / (radius*radius);
        position = initial_pos;
        bcircles.push_back({position, radius});
    }

    void Draw() override {
        Gfx::Circle::Draw(position, radius, Gfx::ColorMaterial{color},
                          GetScene().GetCamera().GetMatrix());
    }

    void Step(float dt) override {
        bcircles[0].center = position;
    }

protected:
    float radius;
    glm::vec4 color;
};

}
