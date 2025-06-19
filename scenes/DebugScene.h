#pragma once

#include "../pixelcore/pixelcore.h"

struct DebugScene : public IScene
{
    struct Pos { float x, y; };
    struct Velocity { float x, y; };
    struct Sprite { tx::view texture; };

    tx::view default_texture;
    ecs::view entity;

    DebugScene() : IScene() {}
 
    void start() override
    {
        default_texture = tx::load("default");

        entity = 
        {
            Pos{50, 50},
            Velocity{300, 300},
            Sprite{{default_texture, {16, 16, 32, 32}}}
        };
    }

    void update() override
    {
        if(HOLD(Z) || HOLD(W)) entity.component<Pos>().y -= entity.component<Velocity>().x * screen::delta;
        if(HOLD(S)) entity.component<Pos>().y += entity.component<Velocity>().x * screen::delta;
        if(HOLD(A) || HOLD(Q)) entity.component<Pos>().x -= entity.component<Velocity>().x * screen::delta;
        if(HOLD(D)) entity.component<Pos>().x += entity.component<Velocity>().x * screen::delta;

        if(HCLICK(LEFT) || CLICK(RIGHT))
        {
            entity.component<Pos>().x = board::cursor.x;
            entity.component<Pos>().y = board::cursor.y;
        }
    }

    void render() override
    {
        entity.component<Sprite>().texture.blit(entity.component<Pos>().x, entity.component<Pos>().y);
    }

    void stop() override
    {
        //...
    }
};