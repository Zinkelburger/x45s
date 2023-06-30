// Copyright Andrew Bernal 2023
#pragma once
#include <iostream>
#include <string>
// Could store 1 card in 1 char to save space, each only needs values 1-13 and 0-4
// That would be an evil monstrosity, but if I need to save space, it could be done...

// These cards are programmed to follow 45s rules for <, >, ==, etc.
// Comparison operators require knowledge of the current trump suit
class Card {
    int value;
    int suit;

 public:
    // I have a default constructor so I can make pairs of cards
    Card() : value(-1000), suit(-1000) {}
    // value, suit
    Card(int inpValue, int inpSuit) : value(inpValue), suit(inpSuit) {}

    friend std::ostream& operator<<(std::ostream& out, const Card& c);

    int getValue() const {
        return value;
    }
    int getSuit() const {
        return suit;
    }
    void setSuit(int inpSuit) {
        suit = inpSuit;
    }
    void setValue(int inpValue) {
        value = inpValue;
    }

    // a comparison function that does not use global variables
    // so you can pass it the suitLed and trump without changing them globally
    bool lessThan(const Card& other, int inpSuit, int inpTrump);

    // overloaded, this one takes a suitLed
    // Returns 1 (true) if the left card is smaller than the right card, and 0 if false
    // Precondition: suitLed is initalized and neither of the cards is trump (checked by operator<)
    int evaluateOffSuit(const Card& other, int suitLed);    
};

// inspired from https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
// operator< is not inline because it is a large function
bool operator<(const Card& lhs, const Card& rhs);
inline bool operator==(const Card& lhs, const Card& rhs) {return lhs.getSuit() == rhs.getSuit()
&& rhs.getValue() == lhs.getValue();}
inline bool operator!=(const Card& lhs, const Card& rhs) {return !operator==(lhs, rhs);}
inline bool operator> (const Card& lhs, const Card& rhs) {return  operator<(rhs, lhs);}
inline bool operator<=(const Card& lhs, const Card& rhs) {return !operator>(lhs, rhs);}
inline bool operator>=(const Card& lhs, const Card& rhs) {return !operator<(lhs, rhs);}
