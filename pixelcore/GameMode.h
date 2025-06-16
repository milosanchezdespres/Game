#pragma once

#include "singleton.h"

#define SCENES GameModeManager::instance()
#define SCENE GameModeManager::instance().scene

//EXAMPLE : GAMEMODE : BATTLE, STAGE : DRAW PHASE
#define GAMEMODE GameModeManager::instance().scene->alias
#define GAMESTAGE GameModeManager::instance().scene->stage

namespace px
{
    struct GameMode
    {
        const char* alias;
        int stage;

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

            void next_stage(){ scene->stage++; }
            void go_to_stage(int new_stage) { scene->stage = new_stage; }
    };
};