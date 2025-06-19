#include "pixelcore/pixelcore.h"

#include "scenes/DebugScene.h"
//...

int main()
{
    board::upload<DebugScene>("debug");
    //...

    return board::run("untitled", 800, 600, "debug");
}
