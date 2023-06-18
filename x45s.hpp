// Copyright Andrew Bernal 2023
#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

namespace Suit {
    enum Suit {
        ACE_OF_HEARTS,
        HEARTS,
        DIAMONDS,
        CLUBS,
        SPADES
    };
}

// https://learn.microsoft.com/en-us/cpp/cpp/extern-cpp?view=msvc-170
// Global variables may be bad, but trump is shared across all of the files
// x45s is the only one who should change trump and suitLed
extern int trump;
extern int suitLed;

namespace x45s {
// These cards are programmed to follow 45s rules for <, >, ==, etc.
// Could store 1 card in 1 char to save space, each only needs values 1-13 and 0-4
// That would be an evil monstrosity, but if I need to save space, it could be done...
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


// player is designed to be overriden by the user
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
    // called after the player has lost the bid. They must keep 1 card
    virtual void discard() = 0;
    // pair is bidAmount, suit
    virtual std::pair<int, Suit::Suit> getBid(const std::vector<int>& bidHistory) = 0;
    // the player is forced to bid
    virtual Suit::Suit bagged() = 0;
    // should return the card you want to play and remove it from your hand
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
    Card evaluate_trick(Card card1, Card card2, Card card3, Card card4);
    Card evaluate_trick(std::vector<Card> c);

    // keep track of the scores. If someone gets 120 points, they win
    void updateScores(int player);

    bool hasWon();
    // Returns the number of the player that has won the game (from 0 to 3).
    // Returns -1 if no one has won
    int whichPlayerWon();
    // calls the player discard method for each player
    void havePlayersDiscard();
    // a getter, not the other thing
    int getBidAmount();
    // sets both the bid amount and the player who bid
    void setBid(int bid, int bidderNum);
    // Calls the getBid method & returns the player who won the bid (0, 1, 2, 3)
    int getBidder();
    // returns the suit of the bid won
    Suit::Suit getBidSuit();

    int getHandSize(int playerNum) {
        return players[playerNum]->getSize();
    }
    int getNumPlayers() {
        return players.size();
    }
    const Player& getPlayer(int playerNum) {
        return *(players[playerNum]);
    }

    bool determineIfWonBidAndDeduct();
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
    std::vector<Card> discardDeck;
    std::vector<int> bidHistory;
    // only two player scores because there are two teams
    int playerScores[2];
    int bidAmount;
    Suit::Suit bidSuit;
    int bidder;
    int bidderInitialScore;
    int playerDealing;
    bool initalizedPlayersWithNew;
};
}  // namespace x45s
