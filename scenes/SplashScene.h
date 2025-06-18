#pragma once

#include "../pixelcore/GameBoard.h"

struct SplashScreen : public IGameLoop
{
    float timer = 0;
    float duration = 2;
    bool ready = false;

    SplashScreen() : IGameLoop() {}

    void start() override
    {
        BGCOLOR = NCOLOR("#000000");

        load_sfx_as("confirm", "ui/confirm");

        play("confirm");
    }

    void update() override
    {

        if(ready && sfx_done("confirm")) GOTO("TitleScene");
        else
        {
            if(timer >= duration) ready = true;
            else timer += DELTA;
        }
    }

    void render() override {}

    void exit() override {}
};
