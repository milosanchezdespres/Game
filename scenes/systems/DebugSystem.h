#pragma once

#include "../../pixelcore/ECS.h"
#include "../../pixelcore/display.h"

#include "../components/Transform.h"

struct DebugSystem : System<Transform>
{
    DebugSystem() : System<Transform>() {}

    void OnUpdate(Entity entity, Transform* component) override
    {
        const char* name = ECS.component<Name>(entity)->value;

        if(name != "test4")
        {
            component->x += component->axis * (250 * delta);
            component->y += component->axis * (250 * delta);

            if(component->x > SCREEN.width || component->y > SCREEN.height) component->axis = -1;
            if(component->x < 0 || component->y < 0) component->axis = 1;

            int R = unpack(component->color)[0];
            int G = unpack(component->color)[1];
            int B = unpack(component->color)[2];
            int A = unpack(component->color)[3];

            if(R > 255) R -= (int) (15 * (1 + delta));
            else R += (int) (1 * (15 + delta));

            if(G > 255) G -= (int) (60 * (1 + delta));
            else G += (int) (60 * (1 + delta));

            if(B > 255) B -= (int) (11 * (1 + delta));
            else B += (int) (1 * (11 + delta));

            component->color = pack(R, G, B, A);
        }
    }
};