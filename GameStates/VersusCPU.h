#pragma once

#include "../pixelcore/ecs/ecs.h"
#include "../pixelcore/GameState.h"

#include "../components/Deck.h"

#include "../cards/cards.h"

enum class VersusCPUPhase
{
    INIT,
    Start,
    Draw,
    PlaceCard,
    Reveal,
    End
};

struct VersusCPU : public GameState
{
    Entity player;
    Entity CPU;

    const int MAX_CARD_COUNT = 8;

    VersusCPU() : GameState()
    {
        data.turn = 0;
        data.current = phase(VersusCPUPhase::INIT);
    }

    void update() override
    {
        switch(data.current)
        {
            case phase(VersusCPUPhase::INIT):
                player = {REG.create_entity()};
                CPU = {REG.create_entity()};

                //FOR NOW WE WILL USE AVAILABLE CARDS TO FILL IN TEST DECK

                player.add<Deck>();
                CPU.add<Deck>();

                player.get<Deck>()->init(MAX_CARD_COUNT);
                CPU.get<Deck>()->init(MAX_CARD_COUNT);

                for(int i = 0; i < MAX_CARD_COUNT; i++)
                {
                    player.get<Deck>()->add_at<LilSkele>(i);
                    CPU.get<Deck>()->add_at<LilSkele>(i);
                }

                data.current = phase(VersusCPUPhase::Start);
                break;

            case phase(VersusCPUPhase::Start):
                // setup for turn
                data.current = phase(VersusCPUPhase::Draw);
                break;

            case phase(VersusCPUPhase::Draw):
                // draw cards
                data.current = phase(VersusCPUPhase::PlaceCard);
                break;

            case phase(VersusCPUPhase::PlaceCard):
                // wait for player action, then
                data.current = phase(VersusCPUPhase::Reveal);
                break;

            case phase(VersusCPUPhase::Reveal):
                // resolve combat or effects
                data.current = phase(VersusCPUPhase::End);
                break;

            case phase(VersusCPUPhase::End):
                data.turn++;
                data.current = phase(VersusCPUPhase::Start);
                break;
        }
    }

    void render() override
    {
        //...
    }
};