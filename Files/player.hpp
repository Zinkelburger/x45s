// Copyright Andrew Bernal 2023
#pragma once
#include <vector>
#include <string>
#include <utility>
#include "card.hpp"
#include "suit.hpp"
// make each player sf::drawable
// player is designed to be overriden by Computer and Human
class Player {
 protected:
    std::vector<Card> hand;

 public:
    Player() {}
    template <class... Cards>
    Player(Cards... cards) : hand{cards...} {}
    Player(std::vector<Card> inpHand) : hand(inpHand) {}
    Player(std::vector<Card>&& inpHand) : hand(inpHand) {}
    // add the card to the player's hand
    void dealCard(Card c) {
        // Card is passed by value so I can simply do this
        hand.push_back(c);
    }
    virtual void discard() = 0;
    // pair is bidAmount, suit
    virtual std::pair<int, Suit::Suit> getBid(const std::vector<int>& bidHistory) = 0;
    virtual Suit::Suit bagged() = 0;
    virtual Card playCard(std::vector<Card> cardsPlayedThisHand) = 0;
    int getSize() {
        return hand.size();
    }
    void resetHand() {
        hand.clear();
    }
    void printHand() {
        for (auto& c : hand) {
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::ostream& printHand(std::ostream& out) {
        for (auto& c : hand) {
            out << c << " ";
        }
        out << "\n";
        return out;
    }
};
