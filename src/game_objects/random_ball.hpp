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
        inverse_inertia = inverse_mass;
        position = initial_pos;
        bcircles.push_back({glm::vec2(), radius});
    }

    void Draw() override {
        Gfx::Circle::Draw(position, radius, Gfx::ColorMaterial{color},
                          GetScene().GetCamera().GetMatrix());
    }

protected:
    float radius;
    glm::vec4 color;
};

}
