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
            Color default_font_color;

        public:
            Font* font;
            nColorArr bgcolor;

            template <typename T> void init
            (
                const char* new_title, int width, int height, 
                const char* _bgcolor = "#091436", const char* _default_font_color = "#ffffff", 
                bool center_window = true, bool resizable = false
            ){
                SCREEN().init(new_title, width, height, center_window, resizable);

                bgcolor = NCOLOR(_bgcolor);
                default_font_color = COLOR(_default_font_color);

                /*load default font //it is possible to use as many as wanted
                //this is just the default one */
                font = new Font("font", default_font_color);

                go<T>();
            }

            template <typename T> void go()
            {
                if(gameloop)
                {
                    gameloop->exit();
                    
                    TEXTURES().clear();
                    ECS().clear();
                }

                gameloop = new T();
                gameloop->start();
            }

            int run()
            {
                while(SCREEN().active())
                {
                    gameloop->update();

                    SCREEN().begin_render(EXPAND(bgcolor));

                    gameloop->render();

                    SCREEN().end_render();
                }

                return SCREEN().exit();
            }
    };
}

using namespace px;//GameBoard is only included once (safe)

DEFINE_SINGLETON_ACCESSOR(GameBoard, BOARD);

#define BGCOLOR BOARD().bgcolor

#define GOTO(T) BOARD().go<T>()