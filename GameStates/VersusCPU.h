#pragma once

#include "../pixelcore/ecs/ecs.h"
#include "../pixelcore/GameState.h"

enum class VersusCPUPhase
{
    Start,
    Draw,
    PlaceCard,
    Reveal,
    End
};

struct VersusCPU : public GameState
{
    VersusCPU() : GameState()
    {
        data.turn = 0;
        data.current = phase(VersusCPUPhase::Start);
    }

    void update() override
    {
        switch(data.current)
        {
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