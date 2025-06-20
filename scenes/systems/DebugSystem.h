#pragma once

#include "../../pixelcore/pixelcore.h"

#include "../entities/DebugEntity.h"

struct DebugSystem : px::ecs::System<DebugFlag, DebugPos, DebugVelocity>
{
    void run(std::tuple<DebugFlag&, DebugPos&, DebugVelocity&> components) override
    {
        auto& [flag, pos, vel] = components;

        if(pos.dragging)
        {
            pos.x = board::cursor.x;
            pos.y = board::cursor.y;
        }
        else
        {
            if(HOLD(Z) || HOLD(W)) pos.y -= vel.x * screen::delta;
            if(HOLD(S)) pos.y += vel.x * screen::delta;
            if(HOLD(A) || HOLD(Q)) pos.x -= vel.x * screen::delta;
            if(HOLD(D)) pos.x += vel.x * screen::delta;
        }

        bool is_mouse_hover = board::hover(pos.x, pos.y, 32, 32);

        if(!pos.dragging && is_mouse_hover && CLICK(LEFT)) pos.dragging = true;
        if(pos.dragging && !HCLICK(LEFT)) pos.dragging = false;
    }
};
