#include "game_objects/scene.hpp"
#include "video/camera.hpp"

using GameObjects::Scene;

Scene::Scene() : camera{*EmplaceGameObject<Video::Camera>()}
{
    scene = this;
}

void Scene::Step(double dt)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->Step(dt);
}

void Scene::Draw()
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->Draw();
}

void Scene::ScreenResized(glm::ivec2 size)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->ScreenResized(size);
}

void Scene::KeyAction(int key, int scancode, int action, int mods)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->KeyAction(key, scancode, action, mods);
}

void Scene::CharTyped(unsigned codepoint)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->CharTyped(codepoint);
}

void Scene::MouseScrolled(glm::dvec2 offset)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->MouseScrolled(offset);
}

void Scene::MouseButtonPressed(int button, int action, int mods)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->MouseButtonPressed(button, action, mods);
}

void Scene::MouseMoved(glm::dvec2 pos)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->MouseMoved(pos);
}


bool Scene::RemoveGameObject(GameObject* game_object)
{
    for (auto i = game_objects.begin(); i != game_objects.end(); ++i) {
        if (i->get() == game_object) {
            game_objects.erase(i);
            return true;
        }
    }

    return false;
}
