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

/*
    start_phase,    0
    draw_phase,     1
    choice_phase,   2
    reveal_phase,   3
    apply_phase,    4
    end_phase       5
*/

struct versusCPU : public GameMode
{
    Entity player;

    Entity test2;
    Entity test3;
    Entity test4;

    versusCPU() : GameMode()
    {
        alias = "versus_cpu";
        stage = 0;
    }
    
    ~versusCPU(){}

    void enter() override
    {
        player = entity::make::debug();
        
        test2 = entity::make::debug();
        test3 = entity::make::debug();
        test4 = entity::make::debug();

        ECS.component<Transform>(player)->y = 100;
        ECS.component<Flags>(player)->type = EntityType::player;

        ECS.component<Transform>(test2)->x = 220;
        ECS.component<Transform>(test4)->x = 320;

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