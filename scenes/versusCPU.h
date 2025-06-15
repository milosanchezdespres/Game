#pragma once

#include "../pixelcore/ECS.h"
#include "../pixelcore/GameMode.h"
#include "../pixelcore/display.h"

using namespace px;

#include <iostream>

struct Transform { float x, y; int scale; };
struct Debug { Color color; };
struct Speed { float move; };

struct versusCPU : public GameMode
{
    Entity player;
    int debug_axis;

    versusCPU() : GameMode() {}
    ~versusCPU(){}

    void enter() override
    {
        player = ECS.create_entity();

        ECS.add_component<Transform>(player);
        ECS.component<Transform>(player)->x = 60;
        ECS.component<Transform>(player)->y = 40;
        ECS.component<Transform>(player)->scale = 50;

        ECS.add_component<Debug>(player);
        ECS.component<Debug>(player)->color = pack("ff00c8");

        ECS.add_component<Speed>(player);
        ECS.component<Speed>(player)->move = 300;

        debug_axis = 1;
    }

    void render() override
    {
        auto transform = ECS.component<Transform>(player);
        auto debug = ECS.component<Debug>(player);
        auto move_speed = ECS.component<Speed>(player)->move;

        transform->x += debug_axis * (move_speed * delta);
        transform->y += debug_axis * (move_speed * delta);

        if(transform->x >= SCREEN.width || transform->y >= SCREEN.height) debug_axis = -1;
        if(debug_axis < 0 && (transform->x < 0 || transform->y < 0)) debug_axis = 1;
        
        SCREEN.blit(transform->x, transform->y, expand2(debug->color), transform->scale);

        //cout << fps << endl;
    }

    void exit() override { ECS.clear(); }
};