#pragma once

#define GAME_STATE GameStateManager::instance().state

namespace px
{
    enum class Phase
    {
        Start,
        Draw,
        PlaceCard,
        Reveal,
        End
    };

    struct GameState
    {
        Phase current = Phase::Start;
        int turn = 0;
    };

    struct GameStateManager : public Singleton<GameStateManager>
    {
        friend class Singleton<GameStateManager>;

        private:
            GameStateManager() = default;

        public:
            GameState state;

            void update()
            {
                switch(state.current)
                {
                    case Phase::Start:
                        // setup for turn
                        state.current = Phase::Draw;
                        break;

                    case Phase::Draw:
                        // draw cards
                        state.current = Phase::PlaceCard;
                        break;

                    case Phase::PlaceCard:
                        // wait for player action, then
                        state.current = Phase::Reveal;
                        break;

                    case Phase::Reveal:
                        // resolve combat or effects
                        state.current = Phase::End;
                        break;

                    case Phase::End:
                        state.turn++;
                        state.current = Phase::Start;
                        break;
                }
            }
    };
}