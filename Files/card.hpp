// Copyright Andrew Bernal 2023
#pragma once
#include <iostream>
#include <string>
// 45s Card is different from a normal card because of the 5 of hearts and the rules
// These cards are programmed to follow 45s rules for <, >, ==, etc.

// Could store 1 card in 1 char to save space, each only needs values 1-13 and 0-4
// That would be an evil monstrosity, but if I need to save space, this is where to look
class Card{
    int value;
    int suit;

 public:
    // Defined input, output, copy assignment, getters, and setters
    // Comparison operators require knowledge of the current trump suit

    // I have a default constructor so I can make pairs of cards
    Card() : value(-1000), suit(-1000) {}
    // value, suit
    Card(int inpValue, int inpSuit) : value(inpValue), suit(inpSuit) {}
    Card(const Card& c);
    friend std::ostream& operator<<(std::ostream& out, const Card& c);
    friend std::istream& operator>>(std::istream& in, Card& c);
    Card& operator=(const Card& inp) {
        value = inp.value;
        suit = inp.suit;
        return *this;
    }
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
};
// inspired from https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
// only operator< is not inline because it is a large function
bool operator<(const Card& lhs, const Card& rhs);
inline bool operator==(const Card& lhs, const Card& rhs) {return lhs.getSuit() == rhs.getSuit()
&& rhs.getValue() == lhs.getValue();}
inline bool operator!=(const Card& lhs, const Card& rhs) {return !operator==(lhs, rhs);}
inline bool operator> (const Card& lhs, const Card& rhs) {return  operator<(rhs, lhs);}
inline bool operator<=(const Card& lhs, const Card& rhs) {return !operator>(lhs, rhs);}
inline bool operator>=(const Card& lhs, const Card& rhs) {return !operator<(lhs, rhs);}