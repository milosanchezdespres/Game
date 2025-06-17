#include "pixelcore/GameBoard.h"
#include "MyGame.h"

int main()
{
    BOARD().init<MyGame>("untitled", 800, 640);

    return BOARD().run();
}
