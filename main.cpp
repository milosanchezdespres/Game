#include <iostream>
using namespace std;

#include "pixelcore/display.h"
#include "pixelcore/GameState.h"

#include "GameStates/VersusCPU.h"

int main()
{
    SCREEN.init("untitled", 800, 680);

    GAMESTATE.transition<VersusCPU>();

    while(SCREEN.active())
    {
        GAMESTATE.update();

        SCREEN.begin_render();

        GAMESTATE.render();

        SCREEN.end_render();
    }

    return SCREEN.exit();
}
