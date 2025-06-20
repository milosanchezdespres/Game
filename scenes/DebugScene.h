#pragma once

#include "../pixelcore/pixelcore.h"

#include "entities/DebugEntity.h"
//...

struct DebugScene : public IScene
{
    tx::view default_texture;

    ecs::view entity2 = nullptr;
    ecs::view entity = nullptr;

    bool dragging = false;
    float drag_offset_x = 0.f;
    float drag_offset_y = 0.f;

    DebugEntityFactory debug_entity_factory;

    DebugScene() : IScene() {}

    void start() override
    {
        default_texture = tx::load("default");

        debug_entity_factory.bake(entity2, default_texture.id());
        debug_entity_factory.bake(entity, default_texture.id());

        entity2.component<Pos>().x = 150;
        entity2.component<Pos>().y = 150;
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
        debug_entity_factory.render(entity);
        debug_entity_factory.render(entity2);
    }

    void stop() override
    {
        //...
    }
};