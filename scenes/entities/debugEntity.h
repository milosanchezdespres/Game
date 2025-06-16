#pragma once

#include "../../pixelcore/ECS.h"
#include "../../pixelcore/colors.h"
#include "../../pixelcore/display.h"

#include "../components/Enabled.h"
#include "../components/Transform.h"
using namespace components;

namespace entity
{
    namespace make
    {
        inline int debug()
        {
            int ID = ECS.create_entity();

            ECS.add_component<Enabled>(ID);
            ECS.component<Enabled>(ID)->value = true;

            ECS.add_component<Transform>(ID);
            ECS.component<Transform>(ID)->color = pack("#ff00c8");
            ECS.component<Transform>(ID)->x = 60;
            ECS.component<Transform>(ID)->y = 40;
            ECS.component<Transform>(ID)->scale = 50;
            ECS.component<Transform>(ID)->axis = 1;

            return ID;
        }
    }

    namespace render
    {
        inline void debug(Entity ID)
        {
            auto transform = ECS.component<Transform>(ID);

            SCREEN.blit(transform->x, transform->y, nexpand(transform->color), transform->scale);
        }
    }
}