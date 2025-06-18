#include "pixelcore/GameBoard.h"

#include "scenes/DebugScene.h"
#include "scenes/SplashScene.h"
#include "scenes/TitleScene.h"

int main()
{
    BOARD().register_scene<DebugScene>("DebugScene");
    BOARD().register_scene<SplashScreen>("SplashScene");
    BOARD().register_scene<TitleScene>("TitleScene");

    BOARD().init("untitled", 800, 640, "SplashScene");

    return BOARD().run();
}
