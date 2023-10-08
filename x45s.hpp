// Copyright Andrew Bernal 2023
#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

namespace Suit {
        enum Suit {
                HEARTS = 1,
                DIAMONDS,
                CLUBS,
                SPADES,
                INVALID
        };
}

namespace x45s {
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
        friend std::ostream& operator<<(std::ostream& out, const Deck& d);
        int getSize() {
                return pack.size();
        }
        // returns a copy of the deck
        std::vector<Card> getPack() {
                return pack;
        }
};

// make each player sf::drawable
// player is designed to be overriden by Computer and Human
class Player {
 protected:
        std::vector<Card> hand;

 public:
        Player() {}
        template <class... Cards>
        explicit Player(Cards... cards) : hand{cards...} {}
        explicit Player(std::vector<Card> inpHand) : hand(inpHand) {}
        virtual ~Player() {}
        // add the card to the player's hand
        void dealCard(Card c) {
                // Card is passed by value so I can simply do this
                hand.push_back(c);
        }
        // the player must keep at least 1 card
        virtual void discard() = 0;
        // pair is bidAmount, suit
        virtual std::pair<int, Suit::Suit> getBid(const std::vector<int>& bidHistory) = 0;
        // the player is forced to bid
        virtual Suit::Suit bagged() = 0;
        // should return the card you want to play and remove it from your hand
        virtual Card playCard(const std::vector<Card>& cardsPlayedThisHand) = 0;
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
        // adds the hand and a trailing '\n'
        std::ostream& printHand(std::ostream& out) {
                for (auto& c : hand) {
                        out << c << " ";
                }
                out << "\n";
                return out;
        }
};
// start with an x because I can't start with a number
class x45s {
 public:
        // no default constructor, have to give it the players at initalization
        x45s() = delete;
        x45s(std::function<Player*()> cp1, std::function<Player*()> cp2,
        std::function<Player*()> cp3, std::function<Player*()> cp4);
        // the user can manage the players' memory if they want to
        x45s(Player* p1, Player* p2, Player* p3, Player* p4);
        ~x45s() {
                if (initalizedPlayersWithNew) {
                        for (auto player : players) {
                                delete player;
                        }
                }
        }
        void deal_players();
        void shuffle();
        void reset();
        // deal the kiddie to the player who won the bid. (0-3)
        void deal_kiddie(int winner);
        // evaluate the trick thrown by all four players. Returns the winning card
        Card evaluate_trick(
                const Card& card1, const Card& card2, const Card& card3, const Card& card4);
        Card evaluate_trick(std::vector<Card> c);

        // Increments the team's score by 5 (team is either 0 or 1)
        void updateScores(int team);

        bool hasWon();
        // Returns the number of the player that has won the game (from 0 to 3).
        // Returns -1 if no one has won
        int whichTeamWon();
        // calls the player discard method for each player
        void havePlayersDiscard();
        // getters
        int getBidAmount() { return bidAmount; }
        int getBidder() { return bidder; }

        // Has each player bid & returns the player who won the bid (0, 1, 2, 3)
        void biddingPhase();
        // returns the trump
        Suit::Suit getTrump() { return trump; }

        int getHandSize(int playerNum) {
                return players[playerNum]->getSize();
        }
        int getNumPlayers() {
                return players.size();
        }
        const Player& getPlayer(int playerNum) {
                return *(players[playerNum]);
        }

        bool deductAfterBid();
        int getTeamScore(int player);

        std::pair<int, bool> dealBidAndFullFiveTricks();

        // returns the cards the players played
        std::vector<Card> havePlayersPlayCards(int playerLeading);
        // have players play their cards and returns the player who won the trick
        std::pair<Card, int> havePlayersPlayCardsAndEvaluate(int playerLeading);

 private:
        Deck deck;
        // Array of pointers to an abstract class
        std::vector<Player*> players;
        // stores the max bid amounts, so players can use it in their decisions
        std::vector<int> bidHistory;
        // only two player scores because there are two teams
        int teamScores[2];
        int teamScoresThisHand[2];

        int bidAmount;
        int bidder;

        Suit::Suit trump;
        Suit::Suit suitLed;

        int playerDealing;
        bool initalizedPlayersWithNew;
};
}
