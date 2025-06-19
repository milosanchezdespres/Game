#include "pixelcore/pixelcore.h"

int main()
{
    screen::init("My Window", 800, 600, true, false, false);

    tex2D default_texture = tx::load("default");

    tx::view tile = {default_texture, {16, 16, 32, 32}}; 

    while (screen::active())
    {
        if (PRESS(ESCAPE)) break;

        print(screen::fps);

        screen::begin_render(GLFW_COLOR("091c36"));

        tile.blit(50, 50);
        tile.blit(150, 50);
        tile.blit(150, 150);
        tile.blit(50, 150);

        screen::end_render();
    }

    return screen::exit();
}