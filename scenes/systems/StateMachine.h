#pragma once

#include "../../pixelcore/ECS.h"
#include "../../pixelcore/display.h"

#include "../components/Flags.h"
#include "../components/State.h"

//changes states based on input, AI logic, etc...

struct StateMachine : System<State>
{
    StateMachine() : System<State>() {}

    void OnUpdate(Entity entity, State* component) override
    {
        auto flags = ECS.component<Flags>(entity);
        auto state_component = ECS.component<State>(entity);

        bool enabled = flags->enabled;
        EntityType type = flags->type;

        StateValue state = state_component->value;

        if(enabled)
        {
            if(GAMEMODE == "versus_cpu")
            {
                switch(GAMESTAGE)
                {
                    case 0://draw phase
                        if(flags->type == EntityType::player) state_component->value = StateValue::idle;
                        else state_component->value = StateValue::debug;

                        //draw cards...

                        break;
                }
            }
        }
    }
};