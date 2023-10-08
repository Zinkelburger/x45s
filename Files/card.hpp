// Copyright Andrew Bernal 2023
#pragma once
#include <iostream>
#include <string>
#include "suit.hpp"
// Could store 1 card in 1 char to save space, each only needs values 1-13 and 0-4 (value and suit)
// That would be an evil monstrosity, but if I need to save space, it could be done...

// These cards are programmed to follow 45s rules for <, >, ==, etc.
// Comparison operators require knowledge of the current trump and suit
class Card {
    int value;
    Suit::Suit suit;
    bool isAceOfHearts;

 public:
    // I need a default constructor so I can make pairs of cards
    Card() : value(-1000), suit(Suit::INVALID) {}
    // value, suit
    Card(int inpValue, int inpSuit) : value(inpValue), suit(static_cast<Suit::Suit>(inpSuit)) {
        isAceOfHearts = value == 1 && suit == Suit::HEARTS;
        value = isAceOfHearts ? 0xACE : value;
    }
    Card(int inpValue, Suit::Suit inpSuit) : value(inpValue), suit(inpSuit) {
        isAceOfHearts = value == 1 && suit == Suit::HEARTS;
        value = isAceOfHearts ? 0xACE : value;
    }

    // outputs the card in string format, e.g. "King of Hearts, "
    friend std::ostream& operator<<(std::ostream& out, const Card& c);

    int getValue() const {
        return value;
    }

    Suit::Suit getSuit() const {
        return suit;
    }

    void setSuit(int inpSuit) {
        suit = static_cast<Suit::Suit>(inpSuit);
    }
    void setSuit(Suit::Suit inpSuit) {
        suit = inpSuit;
    }

    void setValue(int inpValue) {
        value = inpValue;
    }

    bool isTrump(Suit::Suit trump) const {
        return suit == trump || isAceOfHearts;
    }

    bool isSuitLed(Suit::Suit suitLed) const {
        return suit == suitLed;
    }

    // compares two card. Uses evaluateOffSuit if they are offSuit
    friend bool lessThan(
        const Card& card1, const Card& card2, Suit::Suit suitLed, Suit::Suit trump);
    friend bool evaluateOffSuit(const Card& lhs, const Card& rhs, Suit::Suit suitLed);
};

// inspired from https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
inline bool operator==(const Card& lhs, const Card& rhs) { return lhs.getSuit() == rhs.getSuit()
&& rhs.getValue() == lhs.getValue(); }
inline bool operator!=(const Card& lhs, const Card& rhs) { return !operator==(lhs, rhs); }
