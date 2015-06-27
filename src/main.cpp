#include "video/window.hpp"

int main()
{
    Video::Window win(640, 480, "a.out");

    while (!win.IsClosing())
    {
        // blahblah
        win.SwapBuffers();
    }
    return 0;
}
