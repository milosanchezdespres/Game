#pragma once

#include <GL/glew.h>  
#include <GLFW/glfw3.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "external_libraries/stb_image.h"

namespace px
{
    struct Texture
    {
        Texture() : ID(_ID), path(_path), width(_width), height(_height) {}
        Texture(const char* path) : ID(_ID), path(_path), width(_width), height(_height) { load(path); }

    private:
        GLuint _ID = 0;
        std::string _path;
        int _width = 0, _height = 0;

    public:
        const GLuint& ID;
        const std::string& path;
        const int& width;
        const int& height;

        Texture* load(const char* PATH)
        {
            std::string full_path = std::string(PATH) + ".png";
            _path = full_path;

            int channels;
            unsigned char* rawpixels = stbi_load(full_path.c_str(), &_width, &_height, &channels, 4);
            if (!rawpixels)
            {
                std::cout << "error loading file '" << full_path << "'\n";
                return this;
            }

            glGenTextures(1, &_ID);
            glBindTexture(GL_TEXTURE_2D, _ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawpixels);

            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(rawpixels);

            return this;
        }
    };

    struct Surface
    {
        float x, y;
        int width, height;
    };
}
