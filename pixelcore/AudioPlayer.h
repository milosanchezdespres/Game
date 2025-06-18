#pragma once

#define MINIAUDIO_IMPLEMENTATION
#include "external_libraries/miniaudio.h"
#define DR_MP3_IMPLEMENTATION
#include "external_libraries/dr_mp3.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "singleton.h"

const std::string SFX_FOLDER = "assets/SFX";

namespace px
{
    struct AudioPlayer : public Singleton<AudioPlayer>
    {
        friend class Singleton<AudioPlayer>;

        private:
            AudioPlayer() { if (ma_engine_init(nullptr, &engine) != MA_SUCCESS) std::cerr << "Failed to initialize audio engine\n"; }
            ~AudioPlayer() { clear(); ma_engine_uninit(&engine); }

            ma_engine engine;
            std::unordered_map<std::string, std::vector<uint8_t>> raw_data;
            std::unordered_map<std::string, ma_decoder> decoders;
            std::unordered_map<std::string, ma_sound> sounds;

        public:
            void load(const char* alias, const char* path)
            {
                std::string full_path = SFX_FOLDER + "/" + path + ".mp3";
                std::string key(alias);

                if (sounds.count(key)) return;

                FILE* f = fopen(full_path.c_str(), "rb");
                if (!f) { std::cerr << "Failed to open MP3 file: " << full_path << "\n"; return; }

                fseek(f, 0, SEEK_END);
                size_t size = ftell(f);
                fseek(f, 0, SEEK_SET);

                if (size == 0 || size > 100 * 1024 * 1024) { std::cerr << "MP3 file invalid or too large: " << full_path << "\n"; fclose(f); return; }

                std::vector<uint8_t> data(size);
                size_t readSize = fread(data.data(), 1, size, f);
                fclose(f);

                if (readSize != size) { std::cerr << "Failed to read entire MP3 file: " << full_path << "\n"; return; }

                raw_data[key] = std::move(data);

                ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 44100);
                if (ma_decoder_init_memory(raw_data[key].data(), raw_data[key].size(), &config, &decoders[key]) != MA_SUCCESS)
                {
                    std::cerr << "Failed to decode MP3 memory: " << full_path << "\n";
                    raw_data.erase(key);
                    return;
                }

                if (ma_sound_init_from_data_source(&engine, &decoders[key], 0, nullptr, &sounds[key]) != MA_SUCCESS) 
                {
                    std::cerr << "Failed to create sound: " << key << "\n";
                    ma_decoder_uninit(&decoders[key]);
                    decoders.erase(key);
                    raw_data.erase(key);
                    return;
                }

                ma_sound_set_volume(&sounds[key], 1.0f);
                ma_sound_set_looping(&sounds[key], MA_FALSE);
            }

            bool done(const char* alias)
            {
                std::string key(alias);
                if (!sounds.count(key)) return true;
                return ma_sound_is_playing(&sounds[key]) == 0;
            }

            void play(const char* alias)
            {
                std::string key(alias);
                if (sounds.count(key))
                {
                    ma_sound_stop(&sounds[key]);
                    ma_sound_seek_to_pcm_frame(&sounds[key], 0);
                    ma_sound_start(&sounds[key]);
                }
                else std::cerr << "Sound not found: " << alias << "\n";
            }

            void clear()
            {
                for (auto& [_, sound] : sounds) ma_sound_uninit(&sound);
                for (auto& [_, decoder] : decoders) ma_decoder_uninit(&decoder);
                sounds.clear();
                decoders.clear();
                raw_data.clear();
            }
    };
}

using namespace px;
DEFINE_SINGLETON_ACCESSOR(AudioPlayer, SFX);

#define load_sfx(alias) SFX().load(alias, alias);
#define load_sfx_as(alias, path) SFX().load(alias, path);

#define sfx_done(alias) SFX().done(alias)
#define play(alias) SFX().play(alias)
