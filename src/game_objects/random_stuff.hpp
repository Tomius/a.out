#pragma once

#include "gfx/rectangle.hpp"
#include "gfx/material/color_material.hpp"
#include "physics/rigidbody.hpp"

namespace GameObjects {

class RandomStuff : public GameObject, public RigidBody {
public:
    RandomStuff(glm::vec2 initial_pos, float size,
                glm::vec4 color = glm::vec4(1.0f))
            : size(size), color(color) {
        orientation = 17 * M_PI / 180; // initial orientation is 17 degrees
        inverse_mass = 1 / (size*size);
        inverse_inertia = inverse_mass; // todo
        position = initial_pos;

        bboxes.push_back({glm::vec2{-size/2, size/2},
                          glm::vec2{+size/2, 3*size/2}});
        bboxes.push_back({glm::vec2{-size/2, -3*size/2},
                          glm::vec2{+size/2, -size/2}});

        bboxes.push_back({glm::vec2{-3*size/2, -size/2},
                          glm::vec2{-size/2, +size/2}});
        bboxes.push_back({glm::vec2{+size/2, -size/2},
                          glm::vec2{+3*size/2, +size/2}});
    }

    void Draw() const override {
        Gfx::Rectangle::Draw(
            {-size/2, +size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix() * GetMatrix());
        Gfx::Rectangle::Draw(
            {-size/2, -3*size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix() * GetMatrix());

        Gfx::Rectangle::Draw(
            {-3*size/2, -size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix() * GetMatrix());
        Gfx::Rectangle::Draw(
            {+size/2, -size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix() * GetMatrix());
    }

protected:
    float size;
    glm::vec4 color;
};

}
