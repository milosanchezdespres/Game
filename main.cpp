#include "pixelcore/pixelcore.h"

int main()
{
    screen::init("My Window", 800, 600, true, false, false);

    auto default_texture = tx::load("default");
    auto default_texture_blue1 = tx::apply_color(default_texture, COLOR("00defa"));

    print(txd::get(default_texture_blue1)->width);

    while (screen::active())
    {
        if (PRESS(ESCAPE)) break;

        screen::begin_render(GLFW_COLOR("091c36"));

        tx::blit(default_texture, 50, 50);

        tx::blit(default_texture, 50, 350, 50, {0, 0, 32, 32});

        tx::blit(default_texture_blue1, 350, 350, 50);

        screen::end_render();
    }

    return screen::exit();
}