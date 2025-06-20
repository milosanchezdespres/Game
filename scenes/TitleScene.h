#pragma once

#include "../pixelcore/pixelcore.h"

#include "components/Button.h"

struct TitleScene : public IScene
{
    ecs::view UI = nullptr;

    TitleScene() : IScene() {}

    void start() override
    {
        tx::view texbutton1 = tx::load("ui/button1");

        UI $
        UI.add<ButtonHolder>$$

        auto versus_cpu = UI.component<ButtonHolder>().add();
        versus_cpu.text = "versus CPU";
        versus_cpu.texture = texbutton1;
        versus_cpu.texture.surface = {0, 0, texbutton1.width, texbutton1.height / 2};
        versus_cpu.x = 150;
        versus_cpu.y = 150;
    }

    void update() override
    {
        //...
    }

    void render() override
    {
        //...
    }

    void stop() override
    {
        //...
    }
};