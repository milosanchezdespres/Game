#pragma once

#include "../pixelcore/ECS.h"
#include "../pixelcore/GameMode.h"
using namespace px;

#include <iostream>

struct Transform { float x, y; };

struct versusCPU : public GameMode
{
    Entity player;

    versusCPU() : GameMode() {}
    ~versusCPU(){}

    void enter() override
    {
        player = ECS.create_entity();
        ECS.add_component<Transform>(player);
        ECS.component<Transform>(player)->x = 60;
        ECS.component<Transform>(player)->y = 40;
    }

    void render() override
    {
        cout << ECS.component<Transform>(player)->x << endl;
        cout << ECS.component<Transform>(player)->y << endl;
    }

    void exit() override { ECS.clear(); }
};