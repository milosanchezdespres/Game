#include "pixelcore/Colors.h"
#include "pixelcore/Display.h"
#include "pixelcore/Texture.h"
using namespace px;

#include <iostream>

int main()
{
    SCREEN().init("untitled", 800, 580);

    auto bgcolor = NCOLOR("#9d4444");

    Texture* test = Texture().load("assets/default");

    while(SCREEN().active())
    {
        //...

        SCREEN().begin_render(EXPAND(bgcolor));

        //...

        SCREEN().end_render();
    }

    return SCREEN().exit();
}
