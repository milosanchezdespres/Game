#pragma once

#include "pixelcore/GameBoard.h"
#include <deque>
#include <numeric>

//this mess is just debugging font & fps
int axis_x = 1, axis_y = 1;
float x = 0, y = 0;
std::string fps_str;
const char* fps;

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
        x += axis_x * (250 * DELTA);
        y += axis_y * (250 * DELTA);

        if(x > (SCREEN().width - texture("default")->width())) axis_x = -1;
        if(x < -texture("default")->width()) axis_x = 1;

        if(y > (SCREEN().height - texture("default")->height())) axis_y = -1;
        if(y < -texture("default")->height()) axis_y = 1;

        fps_str = std::to_string(static_cast<int>(FPS));
        fps = fps_str.c_str();
    }

    void render() override
    {
        SCREEN().blit(texture("default"), x, y, 2);

        FONT->write(fps, 25, 25, 4);
        FONT->write("hello world", 100, 100, 4);
    }

    void exit() override
    {
        //...
    }
};
