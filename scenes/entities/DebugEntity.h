#pragma once

#include "../../pixelcore/pixelcore.h"

struct Pos { float x, y; };
struct Velocity { float x, y; };
struct Sprite { tx::view texture; };

struct DebugEntityFactory : public ecs::EntityViewFactory
{
    void _on_base_bake_(ecs::view& out_view) override
    {
        out_view.add<Pos>({50, 50});
        out_view.add<Velocity>({300, 300});
        out_view.add<Sprite>({{tx::view(), {16, 16, 32, 32}}});
    }

    void _on_bake_(ecs::view& out_view) override {}

    void _on_bake_with_args(ecs::view& out_view, int texture_id)
    {
        out_view.component<Sprite>().texture.set_id(texture_id);
    }
};
