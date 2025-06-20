#pragma once

#include "../core/core.h"

namespace px
{
    struct Cursor { double x, y; };
    
    struct IScene
    {
        IScene() = default;
        virtual ~IScene() = default;

        virtual void start() {}
        virtual void update() {}
        virtual void render() {}
        virtual void stop() {}
    };

    struct board
    {
        static inline std::unordered_map<std::string, std::unique_ptr<IScene>> _scenes;
        static inline IScene* scene = nullptr;

        static inline bool _initialized = false;
        
        static inline Cursor cursor;
        static inline bool unload_texture_on_wipe = true;

        template<typename T> static void upload(std::string alias)
        {
            auto it = _scenes.find(alias);
            if(it == _scenes.end()) _scenes[alias] = std::make_unique<T>();
        }

        static int run(const char* title, int width, int height, std::string entry_point, bool center_window = true, bool resizable = false, bool borderless = false)
        {
            if(!_initialized)
            {
                screen::init(title, width, height, center_window, resizable, borderless);
                start(entry_point);

                _initialized = true;
            }

            while (screen::active())
            {
                if (PRESS(ESCAPE)) break;

                screen::mouse(cursor.x, cursor.y);

                if(scene)
                {
                    scene->update();
                    ecs::update();
                }

                screen::begin_render(GLFW_COLOR("091c36"));

                if(scene) scene->render();

                screen::end_render();
            }

            wipe();

            if(!unload_texture_on_wipe) tx::clear();

            return screen::exit();
        }

        static void start(std::string alias)
        {
            if(scene) scene->stop();

            wipe();

            auto it = _scenes.find(alias);
            if(it != _scenes.end()) scene = it->second.get();

            if(scene) scene->start();
        }

        static void wipe()
        {
            if (scene) scene->stop();

            ecs::clear();
            if(unload_texture_on_wipe) tx::clear();
        }

        static bool hover(float x, float y, float w, float h)
            { return cursor.x >= x && cursor.x < x + w && cursor.y >= y && cursor.y < y + h; }
    };
}