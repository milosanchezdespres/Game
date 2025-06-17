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

    auto rgba = EXTRACT_RGBA(test.component<Test>()->color);

    std::cout << test.component<Test>()->color << std::endl;
    std::cout << rgba << std::endl;

    return 0;
}
