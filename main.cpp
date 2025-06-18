#include "pixelcore/GameBoard.h"
#include "scenes/DebugScene.h"

int main()
{
    BOARD().init<DebugScene>("untitled", 800, 640);

    return BOARD().run();
}
