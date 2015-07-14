#pragma once

#include "game_objects/static.hpp"
#include "game_objects/scene.hpp"
#include "game_objects/character.hpp"

#include "gfx/material/color_material.hpp"
#include "physics/phy_scene.hpp"

namespace Scenes
{

class MainScene : public GameObjects::Scene, public PhyScene
{
public:
    MainScene() : ground(new RigidBody()) {
        using namespace GameObjects;
        GetCamera().viewport_center = glm::vec2{0, 1};
        GetCamera().viewport_size = 10;

        for (int i = -500; i < +500; i++) {
            glm::vec4 color = glm::vec4(0.3f, 0.65f, 0.2f, 1.0f);
            if (i%2) { color *= 0.75f; }

            EmplaceGameObject<StaticRectangle>(
                Rect<float>{(float)i, 0, 1, -1},
                Gfx::ColorMaterial{color});
        }

        ground->bboxes.push_back({glm::vec2{-500, -1}, glm::vec2{500, 0}});
        AddRigidBody(ground.get());

        AddRigidBody(EmplaceGameObject<Character>());
        AddRigidBody(EmplaceGameObject<RandomBall>(glm::vec2{3.0f, 3.0f}, 1.0f));
        AddRigidBody(EmplaceGameObject<RandomBall>(glm::vec2{-4.0f, 4.0f}, 0.3f));
    }

    void Step(float dt) override {
        Scene::Step(dt);
        PhyScene::Step(dt);
    }

    void Draw() override {
        Scene::Draw();
        PhyScene::DebugDraw(GetScene().GetCamera());
    }
private:
    std::unique_ptr<RigidBody> ground;
};

}
