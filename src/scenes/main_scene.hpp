#pragma once

#include "game_objects/static.hpp"
#include "game_objects/scene.hpp"
#include "game_objects/character.hpp"
#include "game_objects/random_stuff.hpp"

#include "gfx/material/color_material.hpp"
#include "physics/physics_scene.hpp"


namespace Scenes
{

class MainScene : public GameObjects::Scene, public PhysicsScene
{
public:
    MainScene() : ground(new RigidBody()) {
        using namespace GameObjects;
        GetCamera().viewport_center = glm::vec2{0, 1};
        GetCamera().viewport_size = 10;

        // ground
        for (int i = -250; i < +250; i++) {
            glm::vec4 color = glm::vec4(0.3f, 0.65f, 0.2f, 1.0f);
            if (i%2) { color *= 0.75f; }

            EmplaceGameObject<StaticRectangle>(
                Rect<float>{(float)i, -1, 1, 1},
                Gfx::ColorMaterial{color});
        }
        // side walls
        EmplaceGameObject<StaticRectangle>(
            Rect<float>{-250, -1, 1, 51},
            Gfx::ColorMaterial{glm::vec4{1, 1, 1, 1}});
        EmplaceGameObject<StaticRectangle>(
            Rect<float>{249, -1, 1, 51},
            Gfx::ColorMaterial{glm::vec4{1, 1, 1, 1}});

        ground->bounder.boxes.push_back({glm::vec2{-250, -1}, glm::vec2{500, 1}});
        ground->bounder.boxes.push_back({glm::vec2{-250, -1}, glm::vec2{1, 51}});
        ground->bounder.boxes.push_back({glm::vec2{249, -1}, glm::vec2{1, 51}});
        AddRigidBody(ground.get());



        AddRigidBody(EmplaceGameObject<Character>());
        AddRigidBody(EmplaceGameObject<RandomBall>(glm::vec2{3.0f, 3.0f}, 1.2f));
        AddRigidBody(EmplaceGameObject<RandomBall>(glm::vec2{-4.0f, 4.0f}, 0.3f));
        AddRigidBody(EmplaceGameObject<RandomStuff>(glm::vec2{-6.0f, 2.0f}, 0.5f));

        // for (int i = 0; i < 24; ++i) {
        //     AddRigidBody(EmplaceGameObject<RandomBall>(glm::vec2{rand()%24-12, rand()%10+1}, (rand()%10)/20.0f));
        // }
    }

    void Step(float dt) override {
        Scene::Step(dt);
        PhysicsScene::Step(dt);
    }

    void Draw() const override {
        Scene::Draw();
        PhysicsScene::DebugDraw(GetScene().GetCamera());
    }
private:
    std::unique_ptr<RigidBody> ground;
};

}
