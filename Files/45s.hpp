// Copyright Andrew Bernal 2023
#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include "trumpGlobalVariable.hpp"
#include "deck.hpp"
#include "player.hpp"

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
