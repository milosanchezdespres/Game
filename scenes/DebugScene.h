#pragma once

#include "../pixelcore/pixelcore.h"

#include "entities/DebugEntity.h"
//...

struct DebugScene : public IScene
{
    struct Pos { float x, y; };
    struct Velocity { float x, y; };
    struct Sprite { tx::view texture; };

    tx::view default_texture;

    ecs::view removal_test;
    ecs::view entity;

    bool dragging = false;
    float drag_offset_x = 0.f;
    float drag_offset_y = 0.f;

    DebugScene() : IScene() {}

    void start() override
    {
        default_texture = tx::load("default");

        removal_test =
        {
            Pos{150, 150},
            Velocity{300, 300},
            Sprite{{default_texture, {16, 16, 32, 32}}}
        };

        entity =
        {
            Pos{50, 50},
            Velocity{300, 300},
            Sprite{{default_texture, {16, 16, 32, 32}}}
        };

        print(entity.id.version);
        print(removal_test.id.version);
    }

    void update() override
    {
        if(HOLD(Z) || HOLD(W)) entity.component<Pos>().y -= entity.component<Velocity>().x * screen::delta;
        if(HOLD(S)) entity.component<Pos>().y += entity.component<Velocity>().x * screen::delta;
        if(HOLD(A) || HOLD(Q)) entity.component<Pos>().x -= entity.component<Velocity>().x * screen::delta;
        if(HOLD(D)) entity.component<Pos>().x += entity.component<Velocity>().x * screen::delta;

        bool is_mouse_hover = board::hover(
            entity.component<Pos>().x,
            entity.component<Pos>().y,
            entity.component<Sprite>().texture.surface.width,
            entity.component<Sprite>().texture.surface.height
        );

        if(!dragging && is_mouse_hover && CLICK(LEFT))
        {
            dragging = true;
            drag_offset_x = board::cursor.x - entity.component<Pos>().x;
            drag_offset_y = board::cursor.y - entity.component<Pos>().y;
        }

        if(dragging)
        {
            if(HCLICK(LEFT))
            {
                entity.component<Pos>().x = board::cursor.x - drag_offset_x;
                entity.component<Pos>().y = board::cursor.y - drag_offset_y;
            }
            else dragging = false;
        }
    }

    void render() override
    {
        entity.component<Sprite>().texture.blit(entity.component<Pos>().x, entity.component<Pos>().y);
        removal_test.component<Sprite>().texture.blit(removal_test.component<Pos>().x, removal_test.component<Pos>().y);
    }

    void stop() override
    {
        //...
    }
};
