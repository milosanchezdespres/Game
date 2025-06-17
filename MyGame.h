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
        //TO DO : SYSTEMS
    }

    void update() override
    {
        //...

        //print(FPS);
    }

    int axis_x = 1, axis_y = 1;
    float x, y;

    void render() override
    {
        x += axis_x * (250 * DELTA);
        y += axis_y * (250 * DELTA);

        if(x > (SCREEN().width - texture("default")->width())) axis_x = -1;
        if(x < - texture("default")->width()) axis_x = 1;

        if(y > (SCREEN().height - texture("default")->height())) axis_y = -1;
        if(y <  - texture("default")->height()) axis_y = 1;

        SCREEN().blit(texture("default"), x, y, 2);
        SCREEN().blit(texture("default"), 150, 50, 2);
        SCREEN().blit(texture("default"), 150, 150, 2);
        SCREEN().blit(texture("default"), 50, 150, 2);

        //...
    }

    void exit() override
    {
        //...
    }
};