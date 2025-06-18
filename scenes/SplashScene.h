#pragma once

#include "../pixelcore/GameBoard.h"

struct SplashScreen : public IGameLoop
{
    float timer = 0;
    float duration = 1;

    bool ready = false;
    bool play_once = false;

    SplashScreen() : IGameLoop() {}

    void start() override
    {
        BGCOLOR = NCOLOR("#000000");

        load_sfx_as("confirm", "ui/confirm");
    }

    void update() override
    {
        if(ready)
        {
            if(!play_once)
            {
                play("confirm");
                play_once = true;
            }

            if(play_once && sfx_done("confirm")) GOTO("TitleScene");
        }
        else
        {
            if(timer >= duration) ready = true;
            else timer += DELTA;
        }
    }

    void render() override {}

    void exit() override {}
};
