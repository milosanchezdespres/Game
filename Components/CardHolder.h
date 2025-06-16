#pragma once

#include <vector>

enum class CardType { JAN, KEN, PON };

enum class Effect
{
    none
    //....
};

/* not a component */
struct Card
{
    const char* alias;
    int texture_id;
    CardType type;
    Effect effect;
};

/* actual component */
struct CardHolder
{
    private:
        std::vector<Card> cards;

    public:
        CardHolder(){}
        ~CardHolder(){ cards.clear(); }

        void init(int max_card_number) { cards.resize(max_card_number); }
        
        template<typename T> void add() { cards.push_back(T()); }
        template<typename T> T& card(int index) { return cards[index]; }
        void remove(int index) { cards.erase(cards.begin() + index); }

        void clear() {cards.clear(); }
};