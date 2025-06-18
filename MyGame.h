#pragma once

#include "pixelcore/GameBoard.h"
#include <deque>
#include <numeric>

//this mess is just debugging font & fps
int axis_x = 1, axis_y = 1;
float x = 0, y = 0;
float avg_fps;

std::deque<float> fps_values;
static constexpr size_t fps_buffer_size = 10;

struct MyGame : public IGameLoop
{
    MyGame() : IGameLoop() {}

    void init() override
    {
        load_texture("default");

        fps_values.clear();
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

        fps_values.push_back(FPS);
        if (fps_values.size() > fps_buffer_size) fps_values.pop_front();

        avg_fps = std::accumulate(fps_values.begin(), fps_values.end(), 0.0f) / fps_values.size();
    }

    void render() override
    {
        SCREEN().blit(texture("default"), x, y, 2);

        FONT->write(std::to_string(static_cast<int>(avg_fps)).c_str(), 25, 25, 4);
        FONT->write("hello world", 100, 100, 4);
    }

    void exit() override
    {
        //...
    }
};
