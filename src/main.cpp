#include <memory>
#include <vector>
#include "game_objects/static.hpp"
#include "video/window.hpp"

int main()
{
    //Video::Window win(640, 480, "a.out");
    Video::Window win("a.out");

    GameObjects::Scene scene;
    scene.EmplaceGameObject<GameObjects::Static>(Rect<float>{-.5, -.5, .5, .5});
    scene.EmplaceGameObject<GameObjects::Static>(Rect<float>{.8, .8, .1, .15});
    win.SetScene(&scene);

    while (!win.IsClosing())
    {
        glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        scene.Draw();

        win.SwapBuffers();
    }
    return 0;
}
