// Copyright Andrew Bernal 2023
#include "card.hpp"
#include <iostream>
#include <string>
#include <exception>
#include "suit.hpp"

using std::ostream;
using std::istream;
using std::to_string;

// 45s Card is different from a normal card because of the 5 of hearts and the rules
// These cards are programmed to follow 45s rules for <, >, ==, etc.

// Ace of Hearts is set to isAceOfHearts is true

// suits are 1, 2, 3, 4
// 1 = hearts, 2 = diamonds, 3 = clubs, 4 = spades
ostream& operator<<(ostream& out, const Card& c) {
        if (c.getValue() == 13) {
                out << "King";
        } else if (c.getValue() == 12) {
                out << "Queen";
        } else if (c.getValue() == 11) {
                out << "Jack";
        } else if (c.isAceOfHearts) {
                out << "Ace of Hearts, ";
                return out;
        } else if (c.getValue() == 1) {
                out << "Ace";
        } else {
                out << to_string(c.getValue());
        }
        out << " of";
        switch (c.getSuit()) {
                case (Suit::HEARTS): {
                        out << " Hearts, ";
                        break;
                } case (Suit::DIAMONDS): {
                        out << " Diamonds, ";
                        break;
                } case (Suit::CLUBS): {
                        out << " Clubs, ";
                        break;
                } case (Suit::SPADES): {
                        out << " Spades, ";
                        break;
                } default: {
                        throw("Card has an invalid suit!");
                }
        }
        return out;
}

// Precondition: suitLed is initalized and neither of the cards is trump (checked by less than)
bool evaluateOffSuit(const Card& lhs, const Card& rhs, Suit::Suit suitLed) {
        // neither card is suitLed. There is no way to compare them.
        if (lhs.getSuit() != suitLed && rhs.getSuit() != suitLed) {
                return false;
        }

        // now we know one of the cards is of suitLed
        int heartsAndDiamonds[14] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int clubsAndSpades[14] = {13, 12, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int* order;

        if (suitLed == Suit::HEARTS || suitLed == Suit::DIAMONDS) {
                order = heartsAndDiamonds;
        } else {
                order = clubsAndSpades;
        }

        bool lhsSuitLed = lhs.isSuitLed(suitLed);
        bool rhsSuitLed = rhs.isSuitLed(suitLed);

        // both cards are suitLed
        if (lhsSuitLed && rhsSuitLed) {
                for (int i = 0; i < 14; i++) {
                        if (lhs.getValue() == order[i]) {
                                return false;
                        } else if (rhs.getValue() == order[i]) {
                                return false;
                        }
                }
        // right side is suitLed and left is not
        } else if (!lhsSuitLed && rhsSuitLed) {
                return true;
        }
        // left side is suitLed and right is not
        return false;
}

bool lessThan(const Card& lhs, const Card& rhs, Suit::Suit suitLed, Suit::Suit trump) {
        if (!(trump == 1 || trump == 2 || trump == 3 || trump == 4)) {
                throw std::invalid_argument("trump is not valid!");
        }
        // all of the suits, in order from highest to lowest

        // hearts has a -10 at the end so all the arrays can be the same length
        // 0xACE represents the ace of hearts
        int hearts[14] = {5, 11, 0xACE, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2, INT32_MIN};
        int diamonds[14] = {5, 11,  0xACE, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
        int clubsAndSpades[14] = {5, 11, 0xACE, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};

        // order is set to whatever array we need above
        int* order;

        bool lhsTrump = lhs.isTrump(trump);
        bool rhsTrump = rhs.isTrump(trump);

        // neither card is trump
        if (!lhsTrump && !rhsTrump) {
                return evaluateOffSuit(lhs, rhs, suitLed);
        }

        // otherwise comparison is easy, just loop through the order until the highest card is found
        if (trump == Suit::HEARTS) {
                order = hearts;
        } else if (trump == Suit::DIAMONDS) {
                order = diamonds;
        } else {
                order = clubsAndSpades;
        }

        // both cards are trump
        if (lhsTrump && rhsTrump) {
                for (int i = 0; i < 14; i++) {
                        if (lhs.getValue() == order[i]) {
                                return false;
                        } else if (rhs.getValue() == order[i]) {
                                return true;
                        }
                }
        // left card is trump and right is not
        } else if (lhsTrump && !rhsTrump) {
                return false;
        }
        // left side is trump and right is not
        return true;
}
