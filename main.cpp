#include "pixelcore/pixelcore.h"

int main()
{
    screen::init("My Window", 800, 600, true, false, false);

    tx::view tile = {tx::load("default"), {16, 16, 32, 32}}; 
    tx::view tile2 = {tile, {0, 0, 32, 32}}; 

    while (screen::active())
    {
        if (PRESS(ESCAPE)) break;

        screen::begin_render(GLFW_COLOR("091c36"));

        tile.blit(50, 50);
        tile.blit(150, 50);
        tile2.blit(150, 150);
        tile2.blit(50, 150);

        screen::end_render();
    }

    return screen::exit();
}