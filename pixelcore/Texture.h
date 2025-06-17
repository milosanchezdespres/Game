#pragma once

#include <GLFW/glfw3.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "external_libraries/stb_image.h"


namespace px
{
    using Pixels = std::vector<uint32_t>;

    //---------- PNG ONLY ---------------
    struct Texture
    {
        Texture() : ID(_ID), path(_path), width(_width), height(_height) {}
        Texture(const char* path) : ID(_ID), path(_path), width(_width), height(_height) { load(path); }

        private:
            GLuint _ID;
            std::string _path;
            Pixels _pixels;
            int _width, _height;

        public:
            const int& ID;
            const std::string& path;
            const int& width;
            const int& height;

            Texture* load(const char* PATH)
            {
                std::string full_path = ((std::string)(PATH)) + ".png";
                _path = full_path;

                int channels;
                unsigned char* rawpixels = stbi_load(full_path.c_str(), &_width, &_height, &channels, 4);
                if (!rawpixels) std::cout << "error loading file '" + full_path + "'" << std::endl;
                else
                {
                    _pixels.resize(_width * _height);

                    for (int i = 0; i < _width * _height; i++)
                    {
                        unsigned char r = rawpixels[i * 4 + 0];
                        unsigned char g = rawpixels[i * 4 + 1];
                        unsigned char b = rawpixels[i * 4 + 2];
                        unsigned char a = rawpixels[i * 4 + 3];
                        _pixels[i] = (r << 24) | (g << 16) | (b << 8) | a;
                    }

                    glGenTextures(1, &_ID);
                    glBindTexture(GL_TEXTURE_2D, _ID);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels.data());
                    glBindTexture(GL_TEXTURE_2D, 0);

                    stbi_image_free(rawpixels);
                }

                return this;
            }

            unsigned int* pixels() { return _pixels.data(); }
    };
}

namespace std
{
    inline std::ostream& operator<<(std::ostream& os, const px::Texture& tex)
    {
        os << "<texture (" << tex.width << ", " << tex.height << ", " << (tex.width * tex.height) << ") \"" << tex.path << "\">";
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const px::Texture* tex)
    {
        if (tex) os << *tex; else os << "<texture nullptr>";
        return os;
    }
}