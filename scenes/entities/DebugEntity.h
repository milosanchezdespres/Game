#pragma once

#include "../../pixelcore/pixelcore.h"

struct Pos { float x, y; };
struct Velocity { float x, y; };
struct Sprite { tx::view texture; };

struct DebugEntityFactory : public ecs::EntityViewFactory
{
    DebugEntityFactory() {}

    void _on_bake_(ecs::view& out_view) override
    {
        if(!out_view.valid()) out_view = ecs::view{};

        out_view.add<Pos>({50, 50});
        out_view.add<Velocity>({300, 300});
        out_view.add<Sprite>({{tx::view(), {16, 16, 32, 32}}});
    }
};