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

//DEBUG ONLY---------------------
#include "../pixelcore/texture/bmp.h"
//DEBUG ONLY---------------------

struct versusCPU : public GameMode
{
    Entity test1;
    Entity test2;
    Entity test3;

    versusCPU() : GameMode() {}
    ~versusCPU(){}

    void enter() override
    {
        test1 = entity::make::debug();
        test2 = entity::make::debug();
        test3 = entity::make::debug();

        ECS.component<Transform>(test2)->x = 220;
        ECS.component<Transform>(test1)->y = 100;

        ECS.attach<DebugSystem>();

//DEBUG ONLY---------------------
        BMP test = BMP();
        test.load();
        auto pixels = test.render();
//DEBUG ONLY---------------------
    }

    void render() override
    {
        for(auto e: ECS.each<Transform>())
            { entity::render::debug(e); }
    }

    void exit() override { ECS.clear(); }
};