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
struct Deck
{
    private:
        std::vector<Card> cards;

    public:
        Deck(){}
        ~Deck(){ cards.clear(); }

        void init(int max_card_number) { cards.resize(max_card_number); }
        
        template<typename T> void add_at(int index)
        {
            if(index >= 0 && index < cards.size())
                cards[index] = T();
        }
        template<typename T> T& card(int index) { return cards[index]; }
        void remove(int index) { cards.erase(cards.begin() + index); }

        void clear() {cards.clear(); }
        int count() { return cards.size(); }
};