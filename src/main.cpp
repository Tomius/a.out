#include <memory>
#include <vector>
#include "game_objects/static.hpp"
#include "video/window.hpp"

int main()
{
    //Video::Window win(640, 480, "a.out");
    Video::Window win("a.out");
    std::vector<std::unique_ptr<GameObjects::GameObject>> vect;
    vect.push_back(std::make_unique<GameObjects::Static>(
        Rect<float>{-.5, -.5, .5, .5}));

    while (!win.IsClosing())
    {
        glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        for (auto& o : vect)
            o->Draw();

        win.SwapBuffers();
    }
    return 0;
}
