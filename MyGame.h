#pragma once

#include "pixelcore/GameBoard.h"
#include "pixelcore/TextureManager.h"
using namespace px;

struct MyGame : public IGameLoop
{
    MyGame() : IGameLoop() {}

    void init() override
    {
        TEXTURES().load("default", "default");

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
        SCREEN().blit(texture("default"), 50, 50, 2, {0, 0, 32, 32});

        //...
    }

    void exit() override
    {
        //...
    }
};