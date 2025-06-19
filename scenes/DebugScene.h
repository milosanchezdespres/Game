#pragma once

#include "../pixelcore/pixelcore.h"

struct DebugScene : public IScene
{
    struct Pos { float x, y; };
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
            Sprite{{default_texture, {16, 16, 32, 32}}}
        };
    }

    void update() override
    {
        //...
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