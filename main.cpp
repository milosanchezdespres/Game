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
    test.component<Test>()->color = COLOR("#b04830");

    std::cout << test.component<Test>()->color << std::endl;
    std::cout << EXTRACT_RGBA(test.component<Test>()->color) << std::endl;

    return 0;
}
