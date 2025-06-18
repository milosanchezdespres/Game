#pragma once

#include <vector>

//to be able to use Surface struct
#include "../pixelcore/Texture.h"

struct Tileset
{
    //texture ID
    const char* alias;

    int width, height;

    std::vector<Surface> tiles;

    Tileset() { }

    void load(const char* texture_alias, int texture_width, int texture_height, int tilesize)
    {
        tiles.clear();

        alias = texture_alias;

        width = texture_width / tilesize;
        height = texture_height / tilesize;

        tiles.resize(width * height);

        for(int j = 0; j < height; j++)
        {
            for(int i = 0; i < width; i++)
            {
                float x = i * tilesize;
                float y = j * tilesize;

                tiles[j * width + i] = {x, y, tilesize, tilesize};
            }
        }
    }

    Surface& tile(int index) { return tiles[index]; }
};