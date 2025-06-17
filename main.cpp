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

    test.add<Test>({COLOR("#b04830")});

    auto color = test.component<Test>()->color;
    auto rgba = RGBA(test.component<Test>()->color);
    auto normalized = NRGBA(test.component<Test>()->color);

    std::cout << color      << std::endl;
    std::cout << rgba       << std::endl;
    std::cout << normalized << std::endl;

    std::cout << std::endl;

    return 0;
}
