#pragma once

#include "game_objects/static.hpp"
#include "game_objects/scene.hpp"
#include "game_objects/character.hpp"

#include "gfx/material/constant_color_material.hpp"

namespace Scenes
{

class MainScene : public GameObjects::Scene
{
public:
    MainScene() {
        using namespace GameObjects;
        GetCamera().viewport_center = glm::vec2{0, 1};
        GetCamera().viewport_size = 10;

        for (int i = -500; i < +500; i++) {
            glm::vec4 color = glm::vec4(0.3f, 0.65f, 0.2f, 1.0f);
            if (i%2) { color *= 0.75f; }

            EmplaceGameObject<StaticRectangle>(
                Rect<float>{(float)i, 0, 1, -1},
                Gfx::ConstantColorMaterial{color});
        }

        EmplaceGameObject<Character>();
    }
};

}
