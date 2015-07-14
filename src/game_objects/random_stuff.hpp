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
        inverse_mass = 1 / (size*size);
        position = initial_pos;
    }

    void Draw() override {
        Gfx::Rectangle::Draw(
            {position.x - size/2, position.y + size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix());
        Gfx::Rectangle::Draw(
            {position.x - size/2, position.y - 3*size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix());

        Gfx::Rectangle::Draw(
            {position.x - 3*size/2, position.y - size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix());
        Gfx::Rectangle::Draw(
            {position.x + size/2, position.y - size/2, size, size},
            Gfx::ColorMaterial{color},
            GetScene().GetCamera().GetMatrix());
    }

    void Step(float dt) override {
        bboxes.clear();
        bboxes.push_back({position + glm::vec2{-size/2, size/2},
                          position + glm::vec2{+size/2, 3*size/2}});
        bboxes.push_back({position + glm::vec2{-size/2, -3*size/2},
                          position + glm::vec2{+size/2, -size/2}});

        bboxes.push_back({position + glm::vec2{-3*size/2, -size/2},
                          position + glm::vec2{-size/2, +size/2}});
        bboxes.push_back({position + glm::vec2{+size/2, -size/2},
                          position + glm::vec2{+3*size/2, +size/2}});
    }

protected:
    float size;
    glm::vec4 color;
};

}
