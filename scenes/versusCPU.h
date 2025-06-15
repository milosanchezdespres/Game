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
    Entity player;

    versusCPU() : GameMode() {}
    ~versusCPU(){}

    void enter() override
    {
        player = entity::make_debug();
    }

    void render() override
    {
        entity::render_debug(player);
    }

    void exit() override { ECS.clear(); }
};