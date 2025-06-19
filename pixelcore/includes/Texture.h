#pragma once

#include "../core/core.h"

namespace px
{
    struct Surface { float x, y; int width, height; };
    
    struct Texture
    {
        Texture() = default;
        Texture(const char* path) { load(path); }

        Texture(const Texture&) = default;
        Texture(Texture&&) = default;
        Texture& operator=(const Texture&) = default;
        Texture& operator=(Texture&&) = default;

        static Texture* load(const char* PATH)
        {
            return load(PATH, PATH);
        }

        static Texture* load(const std::string& alias, const std::string& path)
        {
            std::string full_path = std::string(TEXDIR + "/" + path) + ".png";
            auto it = _loaded.find(alias);
            if (it != _loaded.end()) return it->second;

            Texture* tex = new Texture();
            tex->_path = full_path;

            int w, h, channels;
            unsigned char* rawpixels = stbi_load(full_path.c_str(), &w, &h, &channels, 4);
            if (!rawpixels)
            {
                std::cout << "error loading file '" << full_path << "'\n";
                delete tex;
                return nullptr;
            }

            tex->_width = float(w);
            tex->_height = float(h);

            glGenTextures(1, &tex->_ID);
            glBindTexture(GL_TEXTURE_2D, tex->_ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawpixels);

            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(rawpixels);

            _loaded[alias] = tex;
            return tex;
        }

        static Texture* apply_color(const std::string& alias, uint32_t color, const std::string& colorName)
        {
            auto it = _loaded.find(alias);
            if (it == _loaded.end()) return nullptr;

            Texture* baseTex = it->second;

            std::string newAlias = alias + "_" + colorName;
            auto itNew = _loaded.find(newAlias);
            if (itNew != _loaded.end()) return itNew->second;

            Texture* newTex = new Texture();
            newTex->_width = baseTex->_width;
            newTex->_height = baseTex->_height;
            newTex->_path = baseTex->_path;

            glGenTextures(1, &newTex->_ID);
            glBindTexture(GL_TEXTURE_2D, newTex->_ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            int size = int(newTex->_width * newTex->_height * 4);
            newTex->_data.resize(size);

            uint8_t cr = (color >> 24) & 0xFF;
            uint8_t cg = (color >> 16) & 0xFF;
            uint8_t cb = (color >> 8)  & 0xFF;
            uint8_t ca = (color >> 0)  & 0xFF;

            for (int i = 0; i < size; i += 4)
            {
                newTex->_data[i + 0] = cr;
                newTex->_data[i + 1] = cg;
                newTex->_data[i + 2] = cb;
                newTex->_data[i + 3] = ca;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, int(newTex->_width), int(newTex->_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, newTex->_data.data());

            glBindTexture(GL_TEXTURE_2D, 0);

            _loaded[newAlias] = newTex;
            return newTex;
        }

        static Texture* apply_tint(const std::string& alias, uint32_t color, float percent, const std::string& tintName)
        {
            if (percent < 0.f) percent = 0.f;
            if (percent > 100.f) percent = 100.f;
            percent /= 100.f;

            auto it = _loaded.find(alias);
            if (it == _loaded.end()) return nullptr;

            Texture* baseTex = it->second;

            std::string newAlias = alias + "_" + tintName;
            auto itNew = _loaded.find(newAlias);
            if (itNew != _loaded.end()) return itNew->second;

            Texture* newTex = new Texture();
            newTex->_width = baseTex->_width;
            newTex->_height = baseTex->_height;
            newTex->_path = baseTex->_path;

            int size = int(newTex->_width * newTex->_height * 4);
            newTex->_data.resize(size);

            glBindTexture(GL_TEXTURE_2D, baseTex->_ID);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, newTex->_data.data());

            uint8_t cr = (color >> 24) & 0xFF;
            uint8_t cg = (color >> 16) & 0xFF;
            uint8_t cb = (color >> 8)  & 0xFF;
            uint8_t ca = (color >> 0)  & 0xFF;

            for (int i = 0; i < size; i += 4)
            {
                float alpha = newTex->_data[i + 3] / 255.f;
                float inv = 1.f - percent;
                newTex->_data[i + 0] = uint8_t(newTex->_data[i + 0] * inv + cr * percent * alpha);
                newTex->_data[i + 1] = uint8_t(newTex->_data[i + 1] * inv + cg * percent * alpha);
                newTex->_data[i + 2] = uint8_t(newTex->_data[i + 2] * inv + cb * percent * alpha);
                newTex->_data[i + 3] = uint8_t(newTex->_data[i + 3] * inv + ca * percent * alpha);
            }

            glGenTextures(1, &newTex->_ID);
            glBindTexture(GL_TEXTURE_2D, newTex->_ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, int(newTex->_width), int(newTex->_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, newTex->_data.data());

            glBindTexture(GL_TEXTURE_2D, 0);

            _loaded[newAlias] = newTex;
            return newTex;
        }

        static Texture* get(const std::string& alias)
        {
            auto it = _loaded.find(alias);
            if (it != _loaded.end()) return it->second;
            return nullptr;
        }

        static void blit(const std::string& alias, float x, float y, float scale = 100.f, Surface surface = { 0, 0, 0, 0 })
        {
            Texture* texture = get(alias);
            if (!texture) return;

            if (surface.width == 0 || surface.height == 0)
            {
                surface.x = 0;
                surface.y = 0;
                surface.width = static_cast<int>(texture->width);
                surface.height = static_cast<int>(texture->height);
            }

            float w = static_cast<float>(surface.width);
            float h = static_cast<float>(surface.height);
            float scaled_w = w * (scale * 0.01f);
            float scaled_h = h * (scale * 0.01f);

            glBindTexture(GL_TEXTURE_2D, texture->ID);

            float u0 = surface.x / texture->width;
            float v0 = surface.y / texture->height;
            float u1 = (surface.x + surface.width) / texture->width;
            float v1 = (surface.y + surface.height) / texture->height;

            glBegin(GL_QUADS);
                glTexCoord2f(u0, v0); glVertex2f(x, y);
                glTexCoord2f(u1, v0); glVertex2f(x + scaled_w, y);
                glTexCoord2f(u1, v1); glVertex2f(x + scaled_w, y + scaled_h);
                glTexCoord2f(u0, v1); glVertex2f(x, y + scaled_h);
            glEnd();

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        const GLuint& ID = _ID;
        const std::string& path = _path;
        
        const float& width = _width;
        const float& height = _height;

    private:
        GLuint _ID = 0;
        std::string _path;

        float _width = 0.f;
        float _height = 0.f;

        std::vector<uint8_t> _data;
        static inline std::unordered_map<std::string, Texture*> _loaded;
    };
}

#define tx Texture
