#include "pixelcore/GameBoard.h"
//#include "scenes/DebugScene.h"
#include "scenes/TitleScene.h"

int main()
{
    BOARD().init<TitleScene>("untitled", 800, 640);

    return BOARD().run();
}
