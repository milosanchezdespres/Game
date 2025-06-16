#include "pixelcore/colors.h"
#include "pixelcore/display.h"
#include "pixelcore/GameMode.h"

#include "scenes/versusCPU.h"

using namespace px;

int main()
{
    SCREEN.init("untitled", 800, 680);

    Color bgcolor = pack("#123248");

    SCENES.go_to<versusCPU>();

    while(SCREEN.active())
    {
        ECS.update();
        
        SCREEN.begin_render(nexpand(bgcolor));

        SCENE->render();

        SCREEN.end_render();
    }

    SCENE->exit();

    return SCREEN.exit();
}
