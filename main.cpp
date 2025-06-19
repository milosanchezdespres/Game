#include "pixelcore/pixelcore.h"

//just for debugging purpose
struct Pos { float x, y; };
struct Sprite { tx::view texture; };

int main()
{
    screen::init("My Window", 800, 600, true, false, false);

    tx::view default_texture = tx::load("default");
    //...

    ecs::view entity = 
    {
        Pos{50, 50},
        Sprite{{default_texture, {16, 16, 32, 32}}}
        //...
    };

    while (screen::active())
    {
        if (PRESS(ESCAPE)) break;

        screen::begin_render(GLFW_COLOR("091c36"));

        entity.component<Sprite>().texture.blit(entity.component<Pos>().x, entity.component<Pos>().y);

        screen::end_render();
    }

    return screen::exit();
}