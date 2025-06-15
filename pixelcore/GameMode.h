#pragma once

#include "singleton.h"

#define SCENES GameModeManager::instance()
#define SCENE GameModeManager::instance().scene

namespace px
{
    struct GameMode
    {
        virtual void enter() {}
        virtual void render() {}
        virtual void exit() {}

        GameMode(){}
        ~GameMode(){}
    };

    struct GameModeManager : public Singleton<GameModeManager>
    {
        friend class Singleton<GameModeManager>;

        public:
            GameMode* scene;
            
            template <typename S>
            void go_to()
            {
                if(scene) scene->exit();
                scene = new S();
                scene->enter();
            }
    };
};