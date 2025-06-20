#pragma once

#include "../pixelcore/pixelcore.h"

#include "entities/DebugEntity.h"
//...

#include "systems/DebugSystem.h"
//...

struct DebugScene : public IScene
{
    tx::view default_texture;

    ecs::view entity2 = nullptr;
    ecs::view entity = nullptr;

    DebugEntityFactory debug_entity_factory;

    DebugScene() : IScene() {}

    void start() override
    {
        default_texture = tx::load("default");

        entity2 = debug_entity_factory.bake(default_texture.id());
        entity = debug_entity_factory.bake(default_texture.id());

        entity2.component<Pos>().x = 150;
        entity2.component<Pos>().y = 150;
        
        ecs::attach<DebugSystem>();
    }

    void update() override {}

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