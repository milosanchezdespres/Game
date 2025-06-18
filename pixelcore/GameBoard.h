#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>
#include <cassert>

#include "Colors.h"
#include "Font.h"
#include "ECS.h"
#include "Display.h"

#define print(var) std::cout << var << std::endl
#define aprint(var) std::cout << var
#define print_new_line std::cout << std::endl;

#define FONT BOARD().font

namespace px
{
    struct IGameLoop
    {
        IGameLoop() = default;
        virtual ~IGameLoop() = default;

        virtual void start() {}
        virtual void update() {}
        virtual void render() {}
        virtual void exit() {}
    };

    struct GameBoard : public Singleton<GameBoard>
    {
        friend class Singleton<GameBoard>;

        private:
            GameBoard() = default;

            std::unique_ptr<IGameLoop> gameloop = nullptr;
            std::unique_ptr<IGameLoop> nextgameloop = nullptr;

            Color default_font_color;
            const char* last_bgcolor = nullptr;

            std::unordered_map<std::string, std::function<std::unique_ptr<IGameLoop>()>> factory_map;

            void private_init()
            {
                bgcolor = NCOLOR(last_bgcolor);
                if (font) delete font;
                font = new Font("font", default_font_color);
            }

        public:
            Font* font = nullptr;
            nColorArr bgcolor;

            template <typename T>
            void register_scene(const std::string& name)
            {
                factory_map[name] = []() -> std::unique_ptr<IGameLoop> {
                    return std::make_unique<T>();
                };
            }

            void init(
                const char* new_title, int width, int height,
                const std::string& start_scene_name,
                const char* _bgcolor = "#091436",
                const char* _default_font_color = "#ffffff",
                bool center_window = true,
                bool resizable = false
            ){
                SCREEN().init(new_title, width, height, center_window, resizable);

                last_bgcolor = _bgcolor;
                default_font_color = COLOR(_default_font_color);

                private_init();

                go(start_scene_name);
            }

            void go(const std::string& scene_name)
            {
                auto it = factory_map.find(scene_name);
                if (it != factory_map.end())
                    nextgameloop = it->second();
                else
                    assert(false && "Scene not registered!");
            }

            void go(std::unique_ptr<IGameLoop> new_game_loop)
            {
                nextgameloop = std::move(new_game_loop);
            }

            int run()
            {
                while (SCREEN().active())
                {
                    if (nextgameloop)
                    {
                        if (gameloop)
                        {
                            gameloop->exit();

                            TEXTURES().clear();
                            ECS().clear();
                        }

                        private_init();

                        gameloop = std::move(nextgameloop);
                        gameloop->start();
                    }
                    else
                    {
                        gameloop->update();

                        SCREEN().begin_render(EXPAND(bgcolor));

                        gameloop->render();

                        SCREEN().end_render();
                    }
                }

                return exit();
            }

            int exit() { return SCREEN().exit(); }
    };
}

using namespace px;

DEFINE_SINGLETON_ACCESSOR(GameBoard, BOARD);

#define BGCOLOR BOARD().bgcolor

#define GOTO(name) BOARD().go(name)

#define EXIT BOARD().exit()
