#pragma once

#define GAMESTATE GameStateManager::instance()
#define phase(phase_enum) static_cast<int>(phase_enum)

namespace px
{
    struct GameStateData
    {
        int current = 0;
        int turn = 0;
    };

    struct GameState
    {
        GameStateData data;
        
        GameState(){}

        virtual void update() {}
        virtual void render() {}
    };

    struct GameStateManager : public Singleton<GameStateManager>
    {
        friend class Singleton<GameStateManager>;

        private:
            GameStateManager() = default;
            GameState state;

        public:
            template<typename T>
            void transition() { state = T(); }

            void update() { state.update(); }
            void render() { state.render(); }
    };
}