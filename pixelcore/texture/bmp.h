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

            palette[0] = pack("#00ffb1");

            pixels.resize(width * height);
        }

        vector<ub32> data()
        {
            int size = width * height;

            auto rgba_pixels = PIXELS(size);

            for(int i = 0; i < size; i++)
                { set_pixel(rgba_pixels, i, palette[pixels[i]]); }

            return rgba_pixels;
        }
    };
}