#pragma once

#include <iostream>

#include "Colors.h"
#include "Font.h"
#include "ECS.h"
#include "Display.h"
//#include "TextureManager.h" already included in font

//for debug purposes
#define print(var) std::cout << var << std::endl
#define aprint(var) std::cout << var
#define print_new_line std::cout << std::endl;

#define FONT BOARD().font

namespace px
{
    struct IGameLoop
    {
        IGameLoop() = default;
        ~IGameLoop() = default;

        virtual void init() {}
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

            IGameLoop* gameloop;
            nColorArr bgcolor;

        public:
            Font* font;

            template <typename T>
            void init(const char* new_title, int width, int height, const char* _bgcolor = "#091436", bool center_window = true, bool resizable = false)
            {
                SCREEN().init(new_title, width, height, center_window, resizable);
                bgcolor = NCOLOR(_bgcolor);

                gameloop = new T();
                gameloop->init();
            }

            int run()
            {
                //load default font
                font = new Font("font");

                gameloop->start();

                while(SCREEN().active())
                {
                    gameloop->update();

                    SCREEN().begin_render(EXPAND(bgcolor));

                    gameloop->render();

                    SCREEN().end_render();
                }

                gameloop->exit();

                ECS().clear();
                TEXTURES().clear();

                return SCREEN().exit();
            }

            //when transitionning to a new scene / game mode etc..
            void clear(bool clear_textures = true)
            {
                if(clear_textures) TEXTURES().clear();
                ECS().clear();
            }
    };
}

using namespace px;//GameBoard is only included once (safe)

DEFINE_SINGLETON_ACCESSOR(GameBoard, BOARD);