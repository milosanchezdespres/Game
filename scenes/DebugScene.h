#pragma once

#include "../pixelcore/GameBoard.h"

#include "../components/Grid.h"
#include "../components/Tileset.h"

int axis_x = 1, axis_y = 1;
float x = 0, y = 0;

struct DebugScene : public IGameLoop
{
    Entity MAP;

    DebugScene() : IGameLoop() {}

    void start() override
    {
        FPS_DISPLAY->scale = 3;

        load_texture("default");
        load_texture("tileset1");

        load_grid(MAP, 16, 5);
        load_tileset("tileset1");
    }

    void update() override
    {
        x += axis_x * (250 * DELTA);
        y += axis_y * (250 * DELTA);

        if(x > (SCREEN().width - (texture("default")->width() * 2))) axis_x = -1;
        if(x < 0) axis_x = 1;

        if(y > (SCREEN().height - (texture("default")->height() * 2))) axis_y = -1;
        if(y < 0) axis_y = 1;

        if (PRESS(SPACE)) GOTO("TitleScene");
    }

    void render() override
    {
        int tileindex = 0;
        float X = 0, Y = 0;

        for(int i = 0; i < 350; i++)
        {
            BLIT_TILE(tileindex, X, Y);

            X += TILESIZE;
            if(X > SCREEN().width - TILESIZE)
            {
                X = 0;
                Y += TILESIZE;
            }

            tileindex++;
            if(tileindex > 5) tileindex = 0;
        }

        BLIT(texture("default"), x, y, 2);

        FONT->write("hello world", 100, 100, 4);
    }

    void exit() override
    {
        axis_x = 1; axis_y = 1;
        x = 0; y = 0;
    }
};
