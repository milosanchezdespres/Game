#include "pixelcore/ECS.h"
using namespace px;

#include <iostream>

struct TestComponent
{
    float x,  y;
};

int main()
{
    Entity test = {ECS().create_entity()};

    test.add<TestComponent>();
    test.component<TestComponent>()->x = 60;

    std::cout << test.component<TestComponent>()->x << std::endl;
}
