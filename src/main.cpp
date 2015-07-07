#include <memory>
#include <vector>

#include "game_objects/static.hpp"
#include "misc/optiongrouper.hpp"
#include "video/window.hpp"

static bool fullscreen;

// todo: remove before release :D
static OptionGrouper::Group foo_group{GetOptionGrouper(), "foo"};
static OptionGrouper::BoolSetOption fullscreen_opt{
    foo_group, fullscreen, true, "fullscreen", "guess what"};

int main(int argc, char** argv)
{
    using namespace GameObjects;

    GetOptionGrouper().Run(argc, argv);

    std::unique_ptr<Video::Window> win;
    if (fullscreen)
        win = std::make_unique<Video::Window>("a.out");
    else
        win = std::make_unique<Video::Window>(640, 480, "a.out");

    Scene scene;
    scene.GetCamera().viewport_center = glm::vec2{5, 5};
    scene.GetCamera().viewport_size = 10;

    scene.EmplaceGameObject<StaticRectangle>(
        Rect<float>{2, 3.5, 2, 2}, glm::vec4(1.0f));
    scene.EmplaceGameObject<StaticRectangle>(
        Rect<float>{7, 7, 0.5f, 1}, glm::vec4{0.8f, 0.65f, 0.03f, 1.0f});
    scene.EmplaceGameObject<StaticPixel>(
        glm::vec2{2, 7}, glm::vec4(1.0));
    scene.EmplaceGameObject<StaticLine>(
        glm::vec2{1, 3}, glm::vec2{9, 2},
        glm::vec4{0.0f, 1.0f, 0.3f, 1.0f});
    scene.EmplaceGameObject<StaticCircle>(
        glm::vec2{8, 4}, 1,
        glm::vec4{0.0f, 0.1f, 0.9f, 1.0f});

    win->SetScene(&scene);

    while (!win->IsClosing())
    {
        glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        scene.Draw();

        win->SwapBuffers();
    }
    return 0;
}
