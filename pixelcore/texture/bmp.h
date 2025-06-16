#pragma once

#include "../colors.h"

namespace px
{
    struct BMP
    {
        public:
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

            vector<ub32>* render()
            {
                int size = width * height;

                _data = PIXELS(size);

                for(int i = 0; i < size; i++)
                    { set_pixel(_data, i, palette[pixels[i]]); }

                return &_data;
            }

        private:
            vector<ub32> _data;
    };
}