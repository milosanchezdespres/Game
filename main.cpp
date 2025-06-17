#include "pixelcore/ECS.h"
using namespace px;

#include <iostream>

struct TestComponent
{
    float x,  y;
};

int main()
{
    Entity test = NEW_ENTITY;

    test.add<TestComponent>();
    test.component<TestComponent>()->x = 60;

    std::cout << test.component<TestComponent>()->x << std::endl;

    return 0;
}
