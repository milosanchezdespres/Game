#pragma once

#include <string>
#include <unordered_map>

#include "singleton.h"
#include "Texture.h"

const std::string FOLDER = "assets/textures";

//access texturre
#define texture(alias) TEXTURES().get(alias)

//loads a texture where path = alias
#define load_texture(alias) TEXTURES().load(alias, alias);

namespace px
{
    struct TextureManager : public Singleton<TextureManager>
    {
        friend class Singleton<TextureManager>;

        private:
            std::unordered_map<std::string, Texture*> textures;
        
        public:
            int load(std::string alias, std::string path)
            {
                std::string full_path = FOLDER + "/" + path;

                Texture* tex = new Texture();
                if (!tex->load(full_path.c_str()))
                {
                    delete tex;
                    return -1;
                }

                textures[alias] = tex;
                return tex->ID();
            }

            Texture* get(std::string alias) { return textures[alias]; }
    };
}

DEFINE_SINGLETON_ACCESSOR(px::TextureManager, TEXTURES);