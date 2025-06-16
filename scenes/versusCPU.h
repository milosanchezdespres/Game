#pragma once

#include "../pixelcore/ECS.h"
#include "../pixelcore/display.h"
#include "../pixelcore/GameMode.h"
using namespace px;

#include "entities/debugEntity.h"
//...

#include "systems/DebugSystem.h"
//...

#include <iostream>

struct versusCPU : public GameMode
{
    Entity test1;
    Entity test2;
    Entity test3;
    Entity test4;

    versusCPU() : GameMode() {}
    ~versusCPU(){}

    void enter() override
    {
        test1 = entity::make::debug("test1");
        test2 = entity::make::debug("test2");
        test3 = entity::make::debug("test3");
        test4 = entity::make::debug("test4");

        ECS.component<Transform>(test2)->x = 220;
        ECS.component<Transform>(test1)->y = 100;

        ECS.attach<DebugSystem>();
    }

    void render() override
    {
        for(auto e: ECS.each<Transform>())
            { entity::render::debug(e); }
    }

    void exit() override { ECS.clear(); }
};