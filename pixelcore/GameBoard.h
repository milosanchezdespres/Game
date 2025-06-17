#pragma once

#include "Colors.h"
#include "ECS.h"
#include "Display.h"

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
                return SCREEN().exit();
            }

            //when transitionning to a new scene / game mode etc..
            void clear() { ECS().clear(); }
    };
}

DEFINE_SINGLETON_ACCESSOR(px::GameBoard, BOARD);