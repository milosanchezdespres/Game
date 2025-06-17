#include "pixelcore/Colors.h"
#include "pixelcore/ECS.h"
#include "pixelcore/Display.h"
using namespace px;

#include <iostream>

struct TestComponent
{
    Texture* texture;
    float x, y;
    int scale;
    Surface surface;
};

//unit test to see if everything works alright
//next step : proper game manager

int main()
{
    SCREEN().init("untitled", 800, 580);

    auto bgcolor = NCOLOR("#091436");

    Entity test = MAKE_ENTITY;
    test.add<TestComponent>({Texture().load("assets/default"), 50, 50, 4, {0, 0, 32, 32}});

    while(SCREEN().active())
    {
        SCREEN().begin_render(EXPAND(bgcolor));

        auto test_component = test.component<TestComponent>();
        SCREEN().blit(test_component->texture, 
            test_component->x, test_component->y, 
            test_component->scale, 
            test_component->surface);

        SCREEN().end_render();
    }

    return SCREEN().exit();
}
