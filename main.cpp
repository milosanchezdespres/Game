#include "pixelcore/GameBoard.h"

#include "scenes/DebugScene.h"
#include "scenes/TitleScene.h"

int main()
{
    BOARD().register_scene<TitleScene>("TitleScene");
    BOARD().register_scene<DebugScene>("DebugScene");

    BOARD().init("untitled", 800, 640, "TitleScene");

    return BOARD().run();
}
