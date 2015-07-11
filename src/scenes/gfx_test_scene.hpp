#pragma once

#include "game_objects/static.hpp"
#include "game_objects/scene.hpp"
#include "gl/texture.hpp"
#include "gfx/material/constant_color_material.hpp"
#include "gfx/material/texture_material.hpp"

namespace Scenes
{

class GfxTestScene : public GameObjects::Scene
{
public:
    GfxTestScene() {
        using namespace GameObjects;
        GetCamera().viewport_center = glm::vec2{5, 5};
        GetCamera().viewport_size = 10;

        Gl::Texture tex;
        tex.Bind();
        tex.LoadImage("test.jpg");
        EmplaceGameObject<StaticRectangle>(
            Rect<float>{2, 3.5, 2, 2}, Gfx::TextureMaterial{std::move(tex)});
        EmplaceGameObject<StaticRectangle>(
            Rect<float>{7, 7, 0.5f, 1}, Gfx::ConstantColorMaterial{
                glm::vec4{0.8f, 0.65f, 0.03f, 1.0f}});
        EmplaceGameObject<StaticPixel>(
            glm::vec2{2, 7}, Gfx::ConstantColorMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0)));
        EmplaceGameObject<StaticLine>(
            glm::vec2{1, 3}, glm::vec2{9, 2},
            glm::vec4{0.0f, 1.0f, 0.3f, 1.0f});
        EmplaceGameObject<StaticCircle>(
            glm::vec2{8, 4}, 1,
            glm::vec4{0.0f, 0.1f, 0.9f, 1.0f});
    }
};

}
