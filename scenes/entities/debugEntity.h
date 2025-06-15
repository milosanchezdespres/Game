#pragma once

#include "../../pixelcore/ECS.h"
#include "../../pixelcore/colors.h"
#include "../../pixelcore/display.h"

#include "../components/Transform.h"
#include "../components/Debug.h"
#include "../components/Speed.h"
#include "../components/Axis.h"
using namespace components;

namespace entity
{
    namespace make
    {
        inline int debug()
        {
            int ID = ECS.create_entity();

            ECS.add_component<Transform>(ID);
            ECS.component<Transform>(ID)->x = 60;
            ECS.component<Transform>(ID)->y = 40;
            ECS.component<Transform>(ID)->scale = 50;

            ECS.add_component<Debug>(ID);
            ECS.component<Debug>(ID)->color = pack(0, 0, 0, 255);

            ECS.add_component<Speed>(ID);
            ECS.component<Speed>(ID)->move = 300;

            ECS.add_component<Axis>(ID);
            ECS.component<Axis>(ID)->value = 1;

            return ID;
        }
    }

    namespace render
    {
        inline void debug(Entity ID)
        {
            auto transform = ECS.component<Transform>(ID);
            auto debug = ECS.component<Debug>(ID);
            auto move_speed = ECS.component<Speed>(ID)->move;
            auto axis = ECS.component<Axis>(ID);

            transform->x += axis->value * (move_speed * delta);
            transform->y += axis->value * (move_speed * delta);

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

            if(transform->x >= SCREEN.width || transform->y >= SCREEN.height) axis->value = -1;
            if(axis->value < 0 && (transform->x < 0 || transform->y < 0)) axis->value = 1;
            
            SCREEN.blit(transform->x, transform->y, nexpand(debug->color), transform->scale);
        }
    }
}