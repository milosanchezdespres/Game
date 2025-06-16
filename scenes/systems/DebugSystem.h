#pragma once

#include "../../pixelcore/ECS.h"
#include "../../pixelcore/display.h"

#include "../components/Transform.h"
#include "../components/Flags.h"

struct DebugSystem : System<Transform>
{
    DebugSystem() : System<Transform>() {}

    void OnUpdate(Entity entity, Transform* component) override
    {
        const char* gamemode = "versus_cpu";

        bool enabled = ECS.component<Flags>(entity)->enabled;
        StateValue state = ECS.component<State>(entity)->value;

        EntityType type =ECS.component<Flags>(entity)->type;

        if(enabled)
        {
            if(gamemode == "versus_cpu" && GAMESTAGE == 0)
            {
                if(type == EntityType::none)
                {
                    switch(state)
                    {
                        case StateValue::idle:
                            component->y += component->axis * (250 * delta);
                            if(component->y > SCREEN.height) component->axis = -1;
                            if(component->y < 0) component->axis = 1;
                            component->color = pack("#ff00c8");
                            break;

                        case StateValue::debug:
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
                            
                            break;
                    }
                }
            }
        }
    }
};