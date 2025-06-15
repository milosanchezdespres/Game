#pragma once

#include "../pixelcore/ECS.h"
#include "../pixelcore/display.h"
#include "../pixelcore/GameMode.h"
using namespace px;

#include "entities/debugEntity.h"
//...

#include <iostream>

struct versusCPU : public GameMode
{
    Entity test1;
    Entity test2;
    Entity test3;

    versusCPU() : GameMode() {}
    ~versusCPU(){}

    void enter() override
    {
        test1 = entity::make_debug();
        test2 = entity::make_debug();
        test3 = entity::make_debug();

        ECS.component<Transform>(test2)->x = 220;
        ECS.component<Transform>(test1)->y = 100;
    }

    void render() override
    {
        for(auto e: ECS.each<Transform>())
            { entity::render_debug(e); }
    }

    void exit() override { ECS.clear(); }
};