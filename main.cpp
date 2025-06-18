#include "pixelcore/GameBoard.h"

#include "scenes/DebugScene.h"
#include "scenes/SplashScene.h"
#include "scenes/TitleScene.h"

int main()
{
    BOARD().upload<DebugScene>("DebugScene");
    BOARD().upload<SplashScene>("SplashScene");
    BOARD().upload<TitleScene>("TitleScene");

    BOARD().init("untitled", "SplashScene");

    return BOARD().run();
}
