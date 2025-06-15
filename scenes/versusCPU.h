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
        ECS.component<Debug>(player)->color = pack(0, 0, 0, 255);

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

        int R = unpack(debug->color)[0];
        int G = unpack(debug->color)[1];
        int B = unpack(debug->color)[2];
        int A = unpack(debug->color)[3];

        if(R > 255) R -= (int) (5 * (1 + delta));
        else R += (int) (5 * (1 + delta));

        if(G > 255) G -= (int) (50 * (1 + delta));
        else G += (int) (50 * (1 + delta));

        if(B > 255) B -= (int) (1 * (1 + delta));
        else B += (int) (1 * (1 + delta));

        debug->color = pack(R, G, B, A);

        if(transform->x >= SCREEN.width || transform->y >= SCREEN.height) debug_axis = -1;
        if(debug_axis < 0 && (transform->x < 0 || transform->y < 0)) debug_axis = 1;
        
        SCREEN.blit(transform->x, transform->y, nexpand(debug->color), transform->scale);

        //cout << fps << endl;
    }

    void exit() override { ECS.clear(); }
};