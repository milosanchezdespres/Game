#include "pixelcore/GameBoard.h"
#include "MyGame.h"

int main()
{
    BOARD().init<MyGame>("untitled", 800, 680);

    return BOARD().run();
}
