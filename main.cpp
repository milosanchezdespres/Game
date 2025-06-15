#include "pixelcore/colors.h"
#include "pixelcore/ECS.h"
#include "pixelcore/Display.h"

using namespace px;

int main()
{
    SCREEN.init("untitled", 800, 480);

    cout << SCREEN.width << endl;

    return 0;
}
