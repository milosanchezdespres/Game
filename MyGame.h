#pragma once

#include "pixelcore/GameBoard.h"
using namespace px;

struct MyGame : public IGameLoop
{
    MyGame() : IGameLoop() {}

    void init() override
    {
        load_texture("default");

        //...
    }

    void start() override
    {
        //...
    }

    void update() override
    {
        //...

        //print(FPS);
    }

    void render() override
    {
        SCREEN().blit(texture("default"), 50, 50, 2);

        //...
    }

    void exit() override
    {
        //...
    }
};