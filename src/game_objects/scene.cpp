#include <queue>
#include "game_objects/scene.hpp"
#include "video/camera.hpp"

using GameObjects::Scene;

Scene::Scene() : camera{*EmplaceGameObject<Video::Camera>()}
{
    scene = this;
}

void Scene::Step(float dt)
{
    for (size_t i = 0; i < game_objects.size(); ++i)
        game_objects[i]->Step(dt);
}


struct GameObjectCompare {
    bool operator()(GameObjects::GameObject* lhs,
                    GameObjects::GameObject* rhs) const {
        return lhs->GetDrawPriority() < rhs->GetDrawPriority();
    }
};

void Scene::Draw() const
{
    std::priority_queue<GameObjects::GameObject*,
                        std::vector<GameObjects::GameObject*>,
                        GameObjectCompare> queue;
    for (size_t i = 0; i < game_objects.size(); ++i) {
        queue.push(game_objects[i].get());
    }
    while(!queue.empty()) {
        queue.top()->Draw();
        queue.pop();
    }
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
    if(grabbingMouse) {
        auto tmp = pos;
        pos = pos - oldCursorPos;
        oldCursorPos = tmp;
    }
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

void Scene::SetWindow(GLFWwindow* window) {
    win = window;
    if(grabbingMouse) {
        double x, y;
        glfwGetCursorPos(win, &x, &y);
        oldCursorPos = {x, y};
    }
}
