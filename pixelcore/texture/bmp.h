#pragma once

#include "../colors.h"

namespace px
{
    struct BMP
    {
        const char* path;

        Palette palette;
        vector<int> pixels;

        int width, height;

        BMP() {}

        void load()
        {
            //...

            width = 5;
            height = 5;
        }

        vector<ub32> render()
        {
            int size = width * height;

            auto pixels = PIXELS(size);

            for(int i = 0; i < size; i++)
                { set_pixel(pixels, i, pack("#00ffb1")); }

            return pixels;
        }
    };
}