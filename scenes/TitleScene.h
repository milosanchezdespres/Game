#pragma once

#include "../pixelcore/pixelcore.h"

struct TitleScene : public IScene
{
    TitleScene() : IScene() {}

    void start() override
    {
        //...
    }

    void update() override
    {
        if(CLICK(RIGHT)) board::start("debug");

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