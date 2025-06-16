#pragma once

#include "../../pixelcore/ECS.h"
#include "../../pixelcore/display.h"

#include "../components/Flags.h"
#include "../components/State.h"

struct StateMachine : System<State>
{
    StateMachine() : System<State>() {}

    void OnUpdate(Entity entity, State* component) override
    {
        bool enabled = ECS.component<Flags>(entity)->enabled;

        if(enabled)
        {
            //...
        }
    }
};