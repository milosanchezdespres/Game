#include "pixelcore/colors.h"
#include "pixelcore/ECS.h"
#include "pixelcore/display.h"

using namespace px;

int main()
{
    SCREEN.init("untitled", 680, 516);

    Color bgcolor = pack(188, 62, 28);

    while(SCREEN.active())
    {
        //...

        SCREEN.begin_render(nexpand(bgcolor));

        //...

        SCREEN.end_render();
    }

    return SCREEN.exit();
}
