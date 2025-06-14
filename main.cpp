#include "pixelcore/colors.h"
#include "pixelcore/ECS.h"

using namespace px;

struct DebugComponent : public IComponent
{
    float x, y;
    DebugComponent() : IComponent() { }
};

int main()
{
    DebugComponent test = DebugComponent();
    test.init(3);
    test.x = 60;

    cout << test.ID << endl;
    cout << test.x << endl;
    
    //...

    return 0;
}
