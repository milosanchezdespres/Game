#include "pixelcore/pixelcore.h"

int main()
{
    screen::init("My Window", 800, 600, true, false, false);

    tex2D default_texture = tx::load("default");

    tx::view default_texture_blue = {default_texture, {16, 16, 32, 32}, COLOR("00c8ff"), 50 }; 

    while (screen::active())
    {
        if (PRESS(ESCAPE)) break;

        screen::begin_render(GLFW_COLOR("091c36"));

        default_texture_blue.blit(50, 50);

        screen::end_render();
    }

    return screen::exit();
}