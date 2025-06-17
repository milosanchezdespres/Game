#include "pixelcore/Colors.h"
#include "pixelcore/Display.h"
using namespace px;

#include <iostream>

int main()
{
    SCREEN().init("untitled", 800, 580);

    auto bgcolor = NCOLOR("#9d4444");

    std::cout << bgcolor << std::endl;

    while(SCREEN().active())
    {
        //...

        SCREEN().begin_render(EXPAND(bgcolor));

        //...

        SCREEN().end_render();
    }

    return SCREEN().exit();
}
