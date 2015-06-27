#include "game_objects/scene.hpp"

using GameObjects::Scene;

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

void Scene::ScreenResized(int width, int height)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->ScreenResized(width, height);
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

void Scene::MouseScrolled(double xoffset, double yoffset)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->MouseScrolled(xoffset, yoffset);
}

void Scene::MouseButtonPressed(int button, int action, int mods)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->MouseButtonPressed(button, action, mods);
}

void Scene::MouseMoved(double xpos, double ypos)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->MouseMoved(xpos, ypos);
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
