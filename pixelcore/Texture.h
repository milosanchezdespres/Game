#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "external_libraries/stb_image.h"

//PNG ONLY

struct Texture
{
    Texture() {}
    Texture(const char* path) { load(path); }

    std::vector<uint32_t> pixels;

    Texture* load(const char* path)
    {
        std::string full_path = ((std::string)(path)) + ".png";

        int width, height, channels;
        unsigned char* rawpixels = stbi_load(full_path.c_str(), &width, &height, &channels, 4);
        if (!rawpixels) std::cout << "error loading file '" + full_path + "'" << std::endl;
        else
        {
            pixels.resize(width * height);

            for (int i = 0; i < width * height; i++)
            {
                unsigned char r = rawpixels[i * 4 + 0];
                unsigned char g = rawpixels[i * 4 + 1];
                unsigned char b = rawpixels[i * 4 + 2];
                unsigned char a = rawpixels[i * 4 + 3];
                pixels[i] = (r << 24) | (g << 16) | (b << 8) | a;
            }
        }

        return this;
    }
};