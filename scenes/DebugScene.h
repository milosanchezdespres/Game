#pragma once

#include "../pixelcore/GameBoard.h"

//this mess is just debugging font & fps
int axis_x = 1, axis_y = 1;
float x = 0, y = 0;
std::string fps_str;
const char* fps;

struct DebugScene : public IGameLoop
{
    DebugScene() : IGameLoop() {}

    void start() override
    {
        load_texture("default");

        //...
    }

    void update() override
    {
        x += axis_x * (250 * DELTA);
        y += axis_y * (250 * DELTA);

        if(x > (SCREEN().width - (texture("default")->width() * 2))) axis_x = -1;
        if(x < 0) axis_x = 1;

        if(y > (SCREEN().height - (texture("default")->height() * 2))) axis_y = -1;
        if(y < 0) axis_y = 1;

        fps_str = std::to_string(static_cast<int>(FPS));
        fps = fps_str.c_str();

        if (PRESS(SPACE)) GOTO("TitleScene");
    }

    void render() override
    {
        BLIT(texture("default"), x, y, 2);

        FONT->write(fps, 25, 25, 3);
        FONT->write("hello world", 100, 100, 4);
    }

    void exit() override
    {
        fps = "";
        axis_x = 1; axis_y = 1;
        x = 0; y = 0;
    }
};
