#pragma once

#include "../Components/Deck.h"

struct LilSkele : public Card
{
    LilSkele() : Card()
    {
        alias = "LilSkele";
        texture_id = 0;
        type = CardType::JAN;
        effect = Effect::none ;
    }
};