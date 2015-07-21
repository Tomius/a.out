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
        inverse_mass = 1 / (M_PI * radius*radius);
        inverse_inertia = inverse_mass / (radius*radius);
        position = initial_pos;
        bounder.circles.push_back({glm::vec2(), radius});
    }

    void Draw() const override {
        Gfx::Circle::Draw(glm::vec2(), radius, Gfx::ColorMaterial{color},
                          GetScene().GetCamera().GetMatrix() * GetMatrix());
    }

protected:
    float radius;
    glm::vec4 color;
};

}
