#include "pixelcore/Colors.h"
#include "pixelcore/Display.h"
using namespace px;

#include <iostream>

int main()
{
    SCREEN().init("untitled", 800, 580);

    auto bgcolor = NCOLOR("#091436");

    Texture* test = Texture().load("assets/default");

    while(SCREEN().active())
    {
        SCREEN().begin_render(EXPAND(bgcolor));

        SCREEN().blit(test, 50, 50);

        SCREEN().end_render();
    }

    return SCREEN().exit();
}
