// Copyright Andrew Bernal 2023
#include "card.hpp"
#include <iostream>
#include <string>
#include <exception>
#include "gameState.hpp"
#include "suit.hpp"

using std::ostream;
using std::istream;
using std::to_string;

// 45s Card is different from a normal card because of the 5 of hearts and the rules
// These cards are programmed to follow 45s rules for <, >, ==, etc.

// Ace of Hearts is set to value: -1, suit: ACE_OF_HEARTS

// suits are 1, 2, 3, 4
// 1 = hearts, 2 = diamonds, 3 = clubs, 4 = spades
ostream& operator<<(ostream& out, const Card& c) {
    if (c.getValue() == 13) {
        out << "King";
    } else if (c.getValue() == 12) {
        out << "Queen";
    } else if (c.getValue() == 11) {
        out << "Jack";
    } else if (c.getSuit() == Suit::ACE_OF_HEARTS) {
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

// a helper function for operator<.
// Returns 1 (true) if the left card is smaller than the right card, and 0 if false
// Precondition: suitLed is initalized and neither of the cards is trump (checked by operator<)
int evaluateOffSuit(const Card& lhs, const Card& rhs) {
    GameState* gameState = GameState::getInstance();

    if (!gameState->getSuitLedInitalized()) {
        throw std::invalid_argument("Suit Led is not initalized!");
    }
    // neither card is suitLed. They will both lose, and there is no way to directly compare them.
    if (lhs.getSuit() != gameState->getSuitLed() && rhs.getSuit() != gameState->getSuitLed()) {
        return -1;
    }

    // now we know one of the cards is of suitLed
    int heartsAndDiamonds[14] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int clubsAndSpades[14] = {13, 12, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* order;
    if (gameState->getSuitLed() == Suit::HEARTS || gameState->getSuitLed() == Suit::DIAMONDS) {
        order = heartsAndDiamonds;
    } else {
        order = clubsAndSpades;
    }

    // both cards are suitLed
    if (lhs.getSuit() == gameState->getSuitLed() && rhs.getSuit() == gameState->getSuitLed()) {
        for (int i = 0; i < 14; i++) {
            if (rhs.getValue() == order[i]) {
                return true;
            } else if (lhs.getValue() == order[i]) {
                return false;
            }
        }
    // right side is suitLed and left is not
    } else if (rhs.getSuit() == gameState->getSuitLed() && lhs.getSuit() != gameState->getSuitLed()) {
        return true;
    // left side is suitLed and right is not
    } else if (rhs.getSuit() != gameState->getSuitLed() && lhs.getSuit() == gameState->getSuitLed()) {
        return false;
    }
    throw("How the hell did you get here");
}

// returns true if a card is higher than another card, considering the trump
// precondition: trump is set. If both cards are offsuite, then suitLed must be set as well
bool operator<(const Card& lhs, const Card& rhs) {
    GameState* gameState = GameState::getInstance();
    if (!gameState->getTrumpInitalized()) {
        throw std::invalid_argument("Trump is not initalized!");
    }
    // all of the suits, in order from highest to lowest

    // hearts has a -10 at the end so all the arrays can be the same length
    // -1 represents the ace of hearts
    int hearts[14] = {5, 11, -1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2, -10};
    int diamonds[14] = {5, 11, -1, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
    int clubsAndSpades[14] = {5, 11, -1, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};

    // order is set to whatever array we need above
    int* order;

    // neither card is trump
    if ((lhs.getSuit() != gameState->getTrump() && lhs.getSuit() != Suit::ACE_OF_HEARTS)
    && (rhs.getSuit() != gameState->getTrump() && rhs.getSuit() != Suit::ACE_OF_HEARTS)) {
        int r = evaluateOffSuit(lhs, rhs);
        // TODO(zinkelburger): Find a better way to resolve "neither card is a winner" situation
        if (r == -1) {
            return false;
        }
        return r;
    }

    // otherwise comparison is easy, just loop through the order until the highest card is found
    if (gameState->getTrump() == Suit::HEARTS) {
        order = hearts;
    } else if (gameState->getTrump() == Suit::DIAMONDS) {
        order = diamonds;
    } else {
        // compiler wants to make sure order is initalized, so I put it in an else
        order = clubsAndSpades;
    }

    // we know at least one of the cards is trump by this point
    // both cards are trump
    if ((lhs.getSuit() == gameState->getTrump() || lhs.getSuit() == Suit::ACE_OF_HEARTS)
    && (rhs.getSuit() == gameState->getTrump() || rhs.getSuit() == Suit::ACE_OF_HEARTS)) {
        for (int i = 0; i < 14; i++) {
            if (rhs.getValue() == order[i]) {
                return true;
            } else if (lhs.getValue() == order[i]) {
                return false;
            }
        }
    // right side is trump and left is not
    } else if ((rhs.getSuit() == gameState->getTrump() || rhs.getSuit() == Suit::ACE_OF_HEARTS)
    && (lhs.getSuit() != gameState->getTrump() && lhs.getSuit() != Suit::ACE_OF_HEARTS)) {
        return true;
    // left side is trump and right is not
    } else if ((rhs.getSuit() != gameState->getTrump() && rhs.getSuit() != Suit::ACE_OF_HEARTS)
    && (lhs.getSuit() == gameState->getTrump() || lhs.getSuit() == Suit::ACE_OF_HEARTS)) {
        return false;
    }
    throw("How the hell did you get here");
}

// a comparison function that does not use global variables
// so you can pass it the suitLed and Trump without changing them globally
// parameter is otherCard, suit, trump
bool Card::lessThan(const Card& other, int inpSuit, int inpTrump) {
    if (!(inpTrump == 0 || inpTrump == 1 || inpTrump == 2 || inpTrump == 3)) {
        throw std::invalid_argument("inpTrump is not valid!");
    }
    // all of the suits, in order from highest to lowest

    // hearts has a -10 at the end so all the arrays can be the same length
    // -1 represents the ace of hearts
    int hearts[14] = {5, 11, -1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2, -10};
    int diamonds[14] = {5, 11, -1, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
    int clubsAndSpades[14] = {5, 11, -1, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};

    // order is set to whatever array we need above
    int* order;

    // neither card is trump
    if ((other.getSuit() != inpTrump && other.getSuit() != Suit::ACE_OF_HEARTS)
    && (this->getSuit() != inpTrump && this->getSuit() != Suit::ACE_OF_HEARTS)) {
        int r = this->evaluateOffSuit(other, inpSuit);
        // TODO(zinkelburger): Find a better way to resolve "neither card is a winner" situation
        if (r == -1) {
            return false;
        }
        return r;
    }

    // otherwise comparison is easy, just loop through the order until the highest card is found
    if (inpTrump == Suit::HEARTS) {
        order = hearts;
    } else if (inpTrump == Suit::DIAMONDS) {
        order = diamonds;
    } else {
        // compiler wants to make sure order is initalized, so I put it in an else
        order = clubsAndSpades;
    }

    // we know at least one of the cards is trump by this point
    // both cards are trump
    if ((other.getSuit() == inpTrump || other.getSuit() == Suit::ACE_OF_HEARTS)
    && (this->getSuit() == inpTrump || this->getSuit() == Suit::ACE_OF_HEARTS)) {
        for (int i = 0; i < 14; i++) {
            if (this->getValue() == order[i]) {
                return true;
            } else if (other.getValue() == order[i]) {
                return false;
            }
        }
    // right side is trump and left is not
    } else if ((this->getSuit() == inpTrump || this->getSuit() == Suit::ACE_OF_HEARTS)
    && (other.getSuit() != inpTrump && other.getSuit() != Suit::ACE_OF_HEARTS)) {
        return true;
    // left side is trump and right is not
    } else if ((this->getSuit() != inpTrump && this->getSuit() != Suit::ACE_OF_HEARTS)
    && (other.getSuit() == inpTrump || other.getSuit() == Suit::ACE_OF_HEARTS)) {
        return false;
    }
    throw("How the hell did you get here");
}

// overloaded, this one takes a inpSuit
// Returns 1 (true) if the left card is smaller than the right card, and 0 if false
int Card::evaluateOffSuit(const Card& other, int inpSuit) {
    if (!(inpSuit == 0 || inpSuit == 1 || inpSuit == 2 || inpSuit == 3)) {
        throw std::invalid_argument("suitLed is not valid!");
    }

    // neither card is inpSuit. They will both lose, and there is no way to directly compare them.
    if (other.getSuit() != inpSuit && this->getSuit() != inpSuit) {
        return -1;
    }

    // now we know one of the cards is of inpSuit
    int heartsAndDiamonds[14] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int clubsAndSpades[14] = {13, 12, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* order;
    if (inpSuit == Suit::HEARTS || inpSuit == Suit::DIAMONDS) {
        order = heartsAndDiamonds;
    } else {
        order = clubsAndSpades;
    }

    // both cards are inpSuit
    if (other.getSuit() == inpSuit && this->getSuit() == inpSuit) {
        for (int i = 0; i < 14; i++) {
            if (this->getValue() == order[i]) {
                return true;
            } else if (other.getValue() == order[i]) {
                return false;
            }
        }
    // right side is inpSuit and left is not
    } else if (this->getSuit() == inpSuit && other.getSuit() != inpSuit) {
        return true;
    // left side is inpSuit and right is not
    } else if (this->getSuit() != inpSuit && other.getSuit() == inpSuit) {
        return false;
    }
    throw("How the hell did you get here");
}