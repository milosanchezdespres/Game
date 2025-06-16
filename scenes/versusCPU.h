#pragma once

#include "../pixelcore/ECS.h"
#include "../pixelcore/display.h"
#include "../pixelcore/GameMode.h"
using namespace px;

#include "entities/debugEntity.h"
//...

#include "components/State.h"
//...

#include "systems/DebugSystem.h"
#include "systems/StateMachine.h"
//...

#include <iostream>

enum class versus_cpu_stage
{
    start_phase,
    draw_phase,
    choice_phase,
    reveal_phase,
    apply_phase,
    end_phase
};

struct versusCPU : public GameMode
{
    Entity test1;
    Entity test2;
    Entity test3;
    Entity test4;

    versusCPU() : GameMode()
    {
        alias = "versus_cpu";
        stage = STAGE_INT(versus_cpu_stage::start_phase);
    }
    
    ~versusCPU(){}

    void enter() override
    {
        test1 = entity::make::debug();
        test2 = entity::make::debug();
        test3 = entity::make::debug();
        test4 = entity::make::debug();

        ECS.component<Transform>(test2)->x = 220;
        ECS.component<Transform>(test1)->y = 100;
        ECS.component<Transform>(test4)->x = 320;

        ECS.component<State>(test4)->value = StateValue::idle;

        ECS.attach<DebugSystem>();
        ECS.attach<StateMachine>();
    }

    void render() override
    {
        for(auto e: ECS.each<Transform>())
            { entity::render::debug(e); }
    }

    void exit() override { ECS.clear(); }
};