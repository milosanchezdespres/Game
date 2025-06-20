#pragma once

#include "../../pixelcore/pixelcore.h"

#include "../entities/DebugEntity.h"

struct DebugSystem : px::ecs::System<Pos, Velocity>
{
    void run(std::tuple<Pos&, Velocity&> components) override
    {
        auto& [pos, vel] = components;

        print(screen::fps);
    }
};