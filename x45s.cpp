// Copyright Andrew Bernal 2023
#include "x45s.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

namespace x45s {
    using std::to_string;
    using std::vector;
    using std::ostream;
    using std::istream;

    void x45s::shuffle() {
        deck.shuffle(10);
    }
    // deal hand to players
    void x45s::deal_players() {
        // make sure each player is dealt until their hand is 5 cards
        // have to use a traditional for loop instead of for-each because I am adding to the vector
        for (unsigned i = 0; i < players.size(); i++) {
            while (players[i]->getSize() < 5) {
                Card c = deck.pop_back();
                players[i]->dealCard(c);
            }
        }
    }
    // ask for bids
    // deal the kiddie to the player who won the bid.
    // Need to input the number of the player who won the bid
    void x45s::deal_kiddie(int winner) {
        if (winner < 0 || winner > 3) {
            throw std::invalid_argument("Invalid winnder of bid. Needs to be player 0, 1, 2, or 3");
        }
        // winner gets 3 cards from the deck
        for (int i = 0; i < 3; i++) {
            players[winner]->dealCard(deck.pop_back());
        }
    }
    // evaluate the trick thrown all four players. Returns the winning card
    Card x45s::evaluate_trick (Card card1, Card card2, Card card3, Card card4) {
        // may be slower than like 15 if statements, but it is very readable
        std::vector<Card> c = {card1, card2, card3, card4};
        return *std::max_element(c.begin(), c.end());
    }   

    // keep track of the scores for each player.
    // If someone gets 120 points, they win and the game ends
    void x45s::updateScores(int player) {
        if(player != 0 && player != 1){
            throw std::invalid_argument("Invalid player " + std::to_string(player) + " in updateScores. Must be 0 or 1");
        }
        playerScores[player] += 5;
    }

    int x45s::getTeamScore(int player) {
        if(player != 0 && player != 1){
            throw std::invalid_argument("Invalid player " + std::to_string(player) + " in updateScores. Must be 0 or 1");
        }
        return playerScores[player];
    }

    bool x45s::hasWon() {
        for(int i = 0; i < 2; i++) {
            // if any player has 120 points or greater, then they have won
            if(playerScores[i] >= 120) {
                return true;
            }
        }
        return false;
    }
    // Returns the number of the player that has won the game (from 0 to 3).
    // Returns -1 if no one has won
    int x45s::whichPlayerWon() {
        for(int i = 0; i<2; i++) {
            // if any player got 120 points or greater, then they have won
            if(playerScores[i] >= 120) {
                return i;
            }
        }
        return -1;
    }
    void x45s::havePlayersDiscard() {
        for (auto& e : players) {
            e->discard();
        }
    }
    int x45s::getBidAmount() {
        return bidAmount;
    }
    // sets both the bid amount and the player who bid
    void x45s::setBid(int bid, int bidderNum) {
        bidAmount = bid;
        bidder = bidderNum;

        // keep track of the bidder's score to detect if they won their bid or not
        bidderInitialScore = playerScores[bidder];
    }
    int x45s::getBidder() {
        std::pair<int, int> what;
        std::pair<int, int> maxWhat = {-2, -2};
        int i = 0;
        int firstPlayer = -1;
        for (auto& e : players) {
            what = e->getBid();
            if (what.first > maxWhat.first) {
                maxWhat = what;
                firstPlayer = i;
            }
            i++;
        }

        // if no player bid, then bag the dealer
        if (maxWhat.first == 0) {
            what = players[playerDealing]->getBid();
            firstPlayer = playerDealing;
            
        }
        playerDealing++;
        playerDealing %= 4;

        bidAmount = maxWhat.first;
        bidSuit = maxWhat.second;

        return firstPlayer;
    }
    int x45s::getBidSuit() {
        return bidSuit;
    }
    bool x45s::determineIfWonBid() {
        // if their (current score - the amount they bid) is greater than or equal to their score before they bid, then they made their bid
        if (playerScores[bidder] - bidAmount < bidderInitialScore) {
            playerScores[bidder] -= bidAmount;
            return false;
        }
        return true;
    }
    void x45s::reset() {
        for (auto& e : players) {
            e->resetHand();
        }
        deck.reset();
    }
    std::vector<Card> x45s::havePlayersPlayCards(int playerLeading) {
        std::vector<Card> cardsPlayed;
        for (int cardNum = playerLeading; cardNum < 4 + playerLeading; cardNum++) {
            cardsPlayed.push_back((*(players[cardNum % 4])).playCard(cardsPlayed));
        }
        return cardsPlayed;
    }
    void x45s::initalizePlayers(Player* p1, Player* p2, Player* p3, Player* p4) {
        players.push_back(p1);
        players.push_back(p2);
        players.push_back(p3);
        players.push_back(p4);
    }

    // 45s Card is different from a normal card because of the 5 of hearts and the rules
    // These cards are programmed to follow 45s rules for <, >, ==, etc.

    // Ace of Hearts is set to value: -1, suit: ACE_OF_HEARTS

    // suits are 1, 2, 3, 4
    // 1 = hearts, 2 = diamonds, 3 = clubs, 4 = spades
    Card::Card(const Card& c) {
        this->value = c.value;
        this->suit = c.suit;
    }
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
    istream& operator>>(istream& in, const Card& c) {
        throw("Function not implemented yet");
        return in;
    }

    // a helper function for operator<.
    // Returns 1 (true) if the left card is smaller than the right card, and 0 if false
    // Precondition: neither of the cards is trump (checked by operator<)
    int evaluateOffSuit(const Card& lhs, const Card& rhs) {
        // neither card is suitLed
        if (lhs.getSuit() != suitLed && rhs.getSuit() != suitLed) {
            // the case where neither card is offsuit, maybe throw an exception
            throw(std::invalid_argument("evaluateOffSuit called with suits: " +
        std::to_string(lhs.getSuit()) + " " +
            std::to_string(rhs.getSuit()) + " and trump is " + std::to_string(trump) + "\n"));
            return -1;
        }

        // one of the cards is of suitLed
        int heartsAndDiamonds[14] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int clubsAndSpades[14] = {13, 12, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int* order;
        if (suitLed == Suit::HEARTS || suitLed == Suit::DIAMONDS) {
            order = heartsAndDiamonds;
        } else {
            order = clubsAndSpades;
        }

        // we know at least one of the cards is suitLed by this point
        // both cards are suitLed
        if (lhs.getSuit() == suitLed && rhs.getSuit() == suitLed) {
            for (int i = 0; i < 14; i++) {
                if (rhs.getValue() == order[i]) {
                    return true;
                } else if (lhs.getValue() == order[i]) {
                    return false;
                }
            }
        // right side is suitLed and left is not
        } else if (rhs.getSuit() == suitLed && lhs.getSuit() != suitLed) {
            return true;
        // left side is suitLed and right is not
        } else if (rhs.getSuit() != suitLed && lhs.getSuit() == suitLed) {
            return false;
        }
        throw("How the hell did you get here");
    }

    // a complicated function, returns true if a card is higher than another card, considering the trump
    bool operator<(const Card& lhs, const Card& rhs) {
        // all of the suits, in order from highest to lowest

        // hearts has a -10 at the end so all the arrays can be the same length
        // -1 represents the ace of hearts
        int hearts[14] = {5, 11, -1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2, -10};
        int diamonds[14] = {5, 11, -1, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
        int clubsAndSpades[14] = {5, 11, -1, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};

        // order is set to whatever array we need above
        int* order;

        // neither card is trump
        if ((lhs.getSuit() != trump && lhs.getSuit() != Suit::ACE_OF_HEARTS)
        && (rhs.getSuit() != trump && rhs.getSuit() != Suit::ACE_OF_HEARTS)) {
            int r = evaluateOffSuit(lhs, rhs);
            // I have evaluateOffSuite throw an exception, but it is something to consider how to solve
            return r;
        }

        // otherwise comparison is easy, just loop through the order until the highest card is found
        if (trump == Suit::HEARTS) {
            order = hearts;
        } else if (trump == Suit::DIAMONDS) {
            order = diamonds;
        } else {
            // compiler wants to make sure order is initalized, so I put it in an else
            order = clubsAndSpades;
        }

        // we know at least one of the cards is trump by this point
        // both cards are trump
        if ((lhs.getSuit() == trump || lhs.getSuit() == Suit::ACE_OF_HEARTS)
        && (rhs.getSuit() == trump || rhs.getSuit() == Suit::ACE_OF_HEARTS)) {
            for (int i = 0; i < 14; i++) {
                if (rhs.getValue() == order[i]) {
                    return true;
                } else if (lhs.getValue() == order[i]) {
                    return false;
                }
            }
        // right side is trump and left is not
        } else if ((rhs.getSuit() == trump || rhs.getSuit() == Suit::ACE_OF_HEARTS)
        && (lhs.getSuit() != trump && lhs.getSuit() != Suit::ACE_OF_HEARTS)) {
            return true;
        // left side is trump and right is not
        } else if ((rhs.getSuit() != trump && rhs.getSuit() != Suit::ACE_OF_HEARTS)
        && (lhs.getSuit() == trump || lhs.getSuit() == Suit::ACE_OF_HEARTS)) {
            return false;
        }
        throw("How the hell did you get here");
    }

    Deck::Deck() {
        // initalize 52 cards, Hearts = 1, Diamonds = 2, Clubs = 3, Spades = 4
        for (int i = Suit::HEARTS; i <= Suit::SPADES; i++) {
            for (int j = 1; j < 14; j++) {
                pack.push_back(Card(j, i));
            }
        }
        // set the ace of hearts to its special suit
        // fortunately this is the first card made (1,1), so I don't have to search for it
        pack[0].setSuit(Suit::ACE_OF_HEARTS);
        pack[0].setValue(-1);
    }
    void Deck::shuffle() {
        // swap each index of the deck with a random index
        // Fischer-Yates
        unsigned int seed = time(nullptr);
        for (int i = pack.size() - 1; i > 0; i--) {
            // TODO(zinkelburger): implement the shuffle with the seed as a parameter
            int j = rand_r(&seed) % (i + 1);
            Card c = pack[j];
            pack[j] = pack[i];
            pack[i] = c;
        }
    }

    void Deck::shuffle(int times) {
        unsigned int seed = time(nullptr);
        for (int j = 0; j < times; j++) {
            // swap each index of the deck with a random index
            for (int i = 0; i < static_cast<int>(pack.size()); i++) {
                int random = rand_r(&seed) % pack.size();
                Card c = pack[random];
                pack[random] = pack[i];
                pack[i] = c;
            }
        }
    }

    Card Deck::pop_back() {
        // I think std::move is overkill as a Card is cheap to copy
        Card c(pack.back());
        pack.pop_back();
        return c;
    }
    Card Deck::peek_back() {
        return pack.back();
    }
    void Deck::push_back(Card c) {
        pack.push_back(c);
    }
    void Deck::reset() {
        pack.clear();
        // the same code as the constructor
        // initalize 52 cards, Hearts = 1, Diamonds = 2, Clubs = 3, Spades = 4
        for (int i = 1; i < 5; i++) {
            for (int j = 1; j < 14; j++) {
                pack.push_back(Card(j, i));
            }
        }

        // set the ace of hearts to -1 value
        // fortunately this is the first card made (1,1)
        pack[0].setValue(-1);
    }

    void Deck::removeCard(const Card& c) {
        // linear search the deck until you find the card, then remove it
        std::vector<Card>::iterator i;
        for (i = pack.begin(); i < pack.end(); i++) {
            if (i->getSuit() == c.getSuit() && i->getValue() == c.getValue()) {
                pack.erase(i);
                break;
            }
        }
        if (i == pack.end()) {
            throw std::invalid_argument("Invalid value, suit pair passed to removeCard: " +
        std::to_string(c.getValue()) + ", " + std::to_string(c.getSuit()) + "\n");
        }
    }

    void Deck::removeCard(int value, int suit) {
        // linear search the deck until you find the card, then remove it
        std::vector<Card>::iterator i;
        for (i = pack.begin(); i < pack.end(); i++) {
            if (i->getSuit() == suit && i->getValue() == value) {
                pack.erase(i);
                return;
            }
        }
        if (i == pack.end()) {
            throw std::invalid_argument("Invalid value, suit pair passed to removeCard: " +
        std::to_string(value) + ", " + std::to_string(suit) + "\n");
        }
    }

    // same code as removeCard, except it doesn't remove the card
    bool Deck::containsCard(int value, int suit) {
        // linear search the deck until you find the card
        std::vector<Card>::iterator i;
        for (i = pack.begin(); i < pack.end(); i++) {
            if (i->getSuit() == suit && i->getValue() == value) {
                return true;
            }
        }
        return false;
    }

    Deck& Deck::operator=(const Deck& other) {
        if (this != &other) {
            pack = other.pack;
        }
        return *this;
    }
    std::ostream& operator<< (std::ostream& out, const Deck& d) {
        for (int i = 0; i < 52; i++) {
            out << d;
            out << " ";
        }
        return out;
    }
}
