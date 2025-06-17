#include "pixelcore/ECS.h"
#include "pixelcore/Colors.h"
using namespace px;

#include <iostream>

struct Test
{
    Color color;
};

int main()
{
    Entity test = MAKE_ENTITY;

    test.add<Test>();
    test.component<Test>()->color = MAKE_COLOR(0, 255, 0, 255);

    std::cout << test.component<Test>()->color << endl;
    std::cout << RGBA(test.component<Test>()->color) << std::endl;

    return 0;
}
