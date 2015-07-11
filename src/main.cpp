#include <memory>
#include <vector>

#include "scenes/gfx_test_scene.hpp"
#include "scenes/main_scene.hpp"
#include "misc/optiongrouper.hpp"
#include "video/window.hpp"

static bool fullscreen;

// todo: remove before release :D
static OptionGrouper::Group foo_group{GetOptionGrouper(), "foo"};
static OptionGrouper::BoolSetOption fullscreen_opt{
    foo_group, fullscreen, true, "fullscreen", "guess what"
};

int main(int argc, char** argv)
{
    RigidBody thingy;
    thingy.AddBounder(BoundingBox());

    using namespace GameObjects;

    GetOptionGrouper().Run(argc, argv);

    std::unique_ptr<Video::Window> win;
    if (fullscreen)
        win = std::make_unique<Video::Window>("a.out");
    else
        win = std::make_unique<Video::Window>(640, 480, "a.out");

    Scenes::GfxTestScene scene;
    //Scenes::MainScene scene;
    win->SetScene(&scene);


    double last_time = glfwGetTime(), current_time, dt;
    while (!win->IsClosing())
    {
        glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        current_time = glfwGetTime();
        dt = current_time - last_time;
        last_time = current_time;

        scene.Step(dt);
        scene.Draw();

        win->SwapBuffers();
    }
    return 0;
}
