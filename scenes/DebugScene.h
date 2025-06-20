#pragma once

#include "../pixelcore/pixelcore.h"

#include "entities/DebugEntity.h"
//...

#include "systems/DebugSystem.h"
//...

struct DebugScene : public IScene
{
    DebugEntityFactory debug_entity_factory;

    DebugScene() : IScene() {}

    void start() override
    {
        tx::view default_texture = tx::load("default");

        debug_entity_factory.bake(default_texture.id(), 50.0f, 50.0f);
        debug_entity_factory.bake(default_texture.id(), 150.0f, 50.0f);
        debug_entity_factory.bake(default_texture.id(), 150.0f, 150.0f);
        debug_entity_factory.bake(default_texture.id(), 50.0f, 150.0f);
        
        ecs::attach<DebugSystem>();
    }

    void update() override
    {
        if(CLICK(RIGHT)) board::start("title");

        //...
    }

    void render() override
    {
        for (auto v : ecs::make_query<DebugFlag>(VIEWTAG)){
            debug_entity_factory.render(v);
        }
    }

    void stop() override
    {
        //...
    }
};