#pragma once

#include "pixelcore/GameBoard.h"

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

        //TO DO : HAVING A CACHE FOR ENTITIES IN GAMEBOARD
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