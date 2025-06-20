#pragma once

#include "../../pixelcore/pixelcore.h"

struct DebugFlag {};
struct DebugPos { float x, y; bool dragging; };
struct DebugVelocity { float x, y; };
struct DebugSprite { tx::view texture; };

struct DebugEntityFactory : public ecs::EntityViewFactory
{
    void _on_base_bake_(ecs::view& out_view) override
    {
        out_view.add<DebugFlag>({});
        out_view.add<DebugPos>({0, 0, false});
        out_view.add<DebugVelocity>({300, 300});
        out_view.add<DebugSprite>({{tx::view(), {16, 16, 32, 32}}});
    }

    void _on_bake_(ecs::view& out_view, ArgsBase* args) override
    {
        if (args)
        {
            //example of several arguments use :
            //auto& [texture_id, speed, name] = static_cast<ArgsPack<int, float, std::string>*>(args)->data;
            
            auto& [texture_id, x, y] = static_cast<ArgsPack<int, float, float>*>(args)->data;

            out_view.component<DebugSprite>().texture.set_id(texture_id);

            out_view.component<DebugPos>().x = x;
            out_view.component<DebugPos>().y = y;
        }
    }

    void _on_render_(ecs::view& out_view) override
    {
        out_view.component<DebugSprite>().texture.blit
        (
            out_view.component<DebugPos>().x, 
            out_view.component<DebugPos>().y
        );
    }
};
