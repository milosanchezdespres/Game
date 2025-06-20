#include "pixelcore/pixelcore.h"

#include "scenes/DebugScene.h"
#include "scenes/TitleScene.h"
//...

int main()
{
    board::upload<DebugScene>("debug");
    board::upload<TitleScene>("title");
    //...

    return board::run("untitled", 800, 600, "title");
}
