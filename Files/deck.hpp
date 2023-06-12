// Copyright Andrew Bernal 2023
#pragma once
#include <vector>
#include "card.hpp"
// store it in a vector so there are easy shuffle and remove operations
class Deck {
 private:
    // idk I needed a word different from deck and card
    std::vector<Card> pack;
 public:
    Deck();
    void shuffle();
    void shuffle(int times);
    Card pop_back();
    Card peek_back();
    void push_back(Card c);
    void reset();
    void removeCard(const Card& c);
    void removeCard(int value, int suit);
    bool containsCard(int value, int suit);
    Deck& operator=(const Deck& other);
    friend std::ostream& operator<< (std::ostream& out, const Deck& d);
};
