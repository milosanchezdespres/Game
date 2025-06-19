#pragma once

#include "../core/core.h"

namespace px
{
    struct Surface { float x, y; int width, height; };

    struct TextureData;
    struct TextureView;

    struct Texture
    {
        static GLuint load(const char* path);
        static GLuint apply_color(GLuint baseID, uint32_t color);
        static GLuint apply_tint(GLuint baseID, uint32_t color, float percent);
        static void blit(GLuint texID, float x, float y, float scale = 100.f, Surface surface = { 0, 0, 0, 0 });

        static void clear();
        static const std::unordered_map<GLuint, TextureData*>& textures();

        //views alones are very lightweight, but careful, color & tint do create a new one
        //you can use another view in the constructor, it will copy the texture ID directly
        using view = TextureView;

        private:
            static inline std::unordered_map<GLuint, TextureData*> _loaded;
            static inline std::vector<uint8_t> _temp;

            friend struct TextureData;
    };

    struct TextureData
    {
        GLuint id = 0;
        float width = 0.f;
        float height = 0.f;

        static TextureData* get(GLuint id)
        {
            auto it = Texture::_loaded.find(id);
            if (it != Texture::_loaded.end()) return it->second;
            return nullptr;
        }
    };

    inline GLuint Texture::load(const char* path)
    {
        std::string full_path = std::string(TEXDIR + "/" + path) + ".png";

        int w, h, channels;
        unsigned char* rawpixels = stbi_load(full_path.c_str(), &w, &h, &channels, 4);
        if (!rawpixels) return 0;

        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawpixels);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(rawpixels);

        TextureData* data = new TextureData;
        data->id = texID;
        data->width = float(w);
        data->height = float(h);
        _loaded[texID] = data;
        return texID;
    }

    inline GLuint Texture::apply_color(GLuint baseID, uint32_t color)
    {
        auto it = _loaded.find(baseID);
        if (it == _loaded.end()) return 0;

        TextureData* base = it->second;

        GLuint newID;
        glGenTextures(1, &newID);
        glBindTexture(GL_TEXTURE_2D, newID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        int size = int(base->width * base->height * 4);
        _temp.resize(size);

        uint8_t cr = (color >> 24) & 0xFF;
        uint8_t cg = (color >> 16) & 0xFF;
        uint8_t cb = (color >> 8)  & 0xFF;
        uint8_t ca = (color >> 0)  & 0xFF;

        for (int i = 0; i < size; i += 4)
        {
            _temp[i + 0] = cr;
            _temp[i + 1] = cg;
            _temp[i + 2] = cb;
            _temp[i + 3] = ca;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, int(base->width), int(base->height), 0, GL_RGBA, GL_UNSIGNED_BYTE, _temp.data());

        glBindTexture(GL_TEXTURE_2D, 0);

        TextureData* newData = new TextureData;
        newData->id = newID;
        newData->width = base->width;
        newData->height = base->height;
        _loaded[newID] = newData;
        return newID;
    }

    inline GLuint Texture::apply_tint(GLuint baseID, uint32_t color, float percent)
    {
        if (percent < 0.f) percent = 0.f;
        if (percent > 100.f) percent = 100.f;
        percent /= 100.f;

        auto it = _loaded.find(baseID);
        if (it == _loaded.end()) return 0;

        TextureData* base = it->second;

        int size = int(base->width * base->height * 4);
        _temp.resize(size);

        glBindTexture(GL_TEXTURE_2D, base->id);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, _temp.data());

        uint8_t cr = (color >> 24) & 0xFF;
        uint8_t cg = (color >> 16) & 0xFF;
        uint8_t cb = (color >> 8)  & 0xFF;
        uint8_t ca = (color >> 0)  & 0xFF;

        float inv = 1.f - percent;
        for (int i = 0; i < size; i += 4)
        {
            _temp[i + 0] = uint8_t(_temp[i + 0] * inv + cr * percent);
            _temp[i + 1] = uint8_t(_temp[i + 1] * inv + cg * percent);
            _temp[i + 2] = uint8_t(_temp[i + 2] * inv + cb * percent);
            _temp[i + 3] = uint8_t(_temp[i + 3] * inv + ca * percent);
        }

        GLuint newID;
        glGenTextures(1, &newID);
        glBindTexture(GL_TEXTURE_2D, newID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, int(base->width), int(base->height), 0, GL_RGBA, GL_UNSIGNED_BYTE, _temp.data());

        glBindTexture(GL_TEXTURE_2D, 0);

        TextureData* newData = new TextureData;
        newData->id = newID;
        newData->width = base->width;
        newData->height = base->height;
        _loaded[newID] = newData;
        return newID;
    }

    inline void Texture::blit(GLuint texID, float x, float y, float scale, Surface surface)
    {
        auto it = _loaded.find(texID);
        if (it == _loaded.end()) return;

        TextureData* texture = it->second;

        if (surface.width == 0 || surface.height == 0)
        {
            surface.x = 0;
            surface.y = 0;
            surface.width = int(texture->width);
            surface.height = int(texture->height);
        }

        float w = float(surface.width);
        float h = float(surface.height);
        float scaled_w = w * (scale * 0.01f);
        float scaled_h = h * (scale * 0.01f);

        glBindTexture(GL_TEXTURE_2D, texture->id);

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

    inline void Texture::clear()
    {
        for (auto& [id, data] : _loaded)
        {
            glDeleteTextures(1, &id);
            delete data;
        }
        _loaded.clear();
        _temp.clear();
    }

    struct TextureView
    {
        private:
            GLuint _id = 0;
            Surface _surface;
            float _scale = 100.f;
            float _width = 0.f;
            float _height = 0.f;

        public:
            const float& width = _width;
            const float& height = _height;
            Surface& surface = _surface;
            float& scale = _scale;

            TextureView(GLuint id = 0, Surface surface = {0, 0, 0, 0}, float scale = 100.f)
                : _id(id), _surface(surface), _scale(scale),
                width(_width), height(_height)
            {
                _width = float(surface.width);
                _height = float(surface.height);
            }

            TextureView(const TextureView& view)
                : _id(view._id), _surface(view._surface), _scale(view._scale),
                width(_width), height(_height)
            {
                _width = float(_surface.width);
                _height = float(_surface.height);
            }

            TextureView(const TextureView& view, Surface surface, float scale = 100.f)
                : _id(view._id), _surface(surface), _scale(scale),
                width(_width), height(_height)
            {
                _width = float(surface.width);
                _height = float(surface.height);
            }

            TextureView(GLuint id, Surface surface, float scale, uint32_t color)
                : _surface(surface), _scale(scale),
                width(_width), height(_height)
            {
                _id = Texture::apply_color(id, color);
                _width = float(surface.width);
                _height = float(surface.height);
            }

            TextureView(GLuint id, Surface surface, float scale, uint32_t color, float percent)
                : _surface(surface), _scale(scale),
                width(_width), height(_height)
            {
                _id = Texture::apply_tint(id, color, percent);
                _width = float(surface.width);
                _height = float(surface.height);
            }

            void blit(float x, float y) const { Texture::blit(_id, x, y, _scale, _surface); }

            void source(Surface surface)
            {
                _surface = surface;
                _width = float(surface.width);
                _height = float(surface.height);
            }

            GLuint id() const { return _id; }
    };

    inline const std::unordered_map<GLuint, TextureData*>& Texture::textures() { return _loaded; }
}

#define tx Texture
#define txd TextureData
#define tex2D GLuint

#define all_textures auto& [ID, DATA] : tx::textures()
