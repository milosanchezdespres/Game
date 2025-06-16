#include <iostream>
using namespace std;

#include "pixelcore/ecs/ecs.h"

struct CardComponent
{
    const char* text = "...";
};

int main()
{
    Entity test = {REG.create_entity()};

    test.add<CardComponent>();

    cout << test.get<CardComponent>()->text << endl;

    return 0;
}
