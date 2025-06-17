#include "pixelcore/Colors.h"
#include "pixelcore/Display.h"
#include "pixelcore/Texture.h"
using namespace px;

#include <iostream>

int main()
{
    SCREEN().init("untitled", 800, 580);

    auto bgcolor = NCOLOR("#091436");

    Texture* test = Texture().load("assets/default");

    std::cout << test->width << std::endl;
    std::cout << test->height << std::endl;

    while(SCREEN().active())
    {
        //...

        SCREEN().begin_render(EXPAND(bgcolor));

        //...

        SCREEN().end_render();
    }

    return SCREEN().exit();
}
