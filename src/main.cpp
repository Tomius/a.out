#include <memory>
#include <vector>
#include "game_objects/static_rectangle.hpp"
#include "video/window.hpp"

int main()
{
    //Video::Window win(640, 480, "a.out");
    Video::Window win("a.out");

    GameObjects::Scene scene;
    using GameObjects::StaticRectangle;
    scene.EmplaceGameObject<StaticRectangle>(
        Rect<float>{-.5, -.5, .5, .5}, glm::vec4(1.0f));
    scene.EmplaceGameObject<StaticRectangle>(
        Rect<float>{.8, .8, .1, .15}, glm::vec4{0.8f, 0.65f, 0.03f, 1.0f});
    win.SetScene(&scene);

    while (!win.IsClosing())
    {
        glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        scene.Draw();

        win.SwapBuffers();
    }
    return 0;
}
