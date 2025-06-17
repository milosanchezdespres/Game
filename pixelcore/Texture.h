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
        Texture() = default;
        Texture(const char* path) { load(path); }

        Texture(const Texture&) = default;
        Texture(Texture&&) = default;
        Texture& operator=(const Texture&) = default;
        Texture& operator=(Texture&&) = default;

        Texture* load(const char* PATH)
        {
            std::string full_path = std::string(PATH) + ".png";
            _path = full_path;

            int channels;
            unsigned char* rawpixels = stbi_load(full_path.c_str(), &_width, &_height, &channels, 4);
            if (!rawpixels)
            {
                std::cout << "error loading file '" << full_path << "'\n";
                return nullptr;
            }

            glGenTextures(1, &_ID);
            glBindTexture(GL_TEXTURE_2D, _ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawpixels);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(rawpixels);

            return this;
        }

        GLuint ID() const { return _ID; }

        const std::string& path() const { return _path; }

        int width() const { return _width; }
        int height() const { return _height; }

        private:
            GLuint _ID = 0;
            std::string _path;
            int _width = 0, _height = 0;
    };

    struct Surface
    {
        float x, y;
        int width, height;
    };
}
