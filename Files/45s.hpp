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
    std::function<Player*()> cp3, std::function<Player*()> cp4) : deck() {
        initalizedPlayersWithNew = true;
        players.push_back(cp1());
        players.push_back(cp2());
        players.push_back(cp3());
        players.push_back(cp4());

        // initalize both the player scores to 0
        playerScores[0] = 0;
        playerScores[1] = 0;
        // player 0 can deal first. This is incremented mod 4 after every deal
        playerDealing = 0;
    }
    // the user can manage the players if they want to
    x45s(Player* p1, Player* p2, Player* p3, Player* p4) : deck() {
        initalizedPlayersWithNew = false;
        players.push_back(p1);
        players.push_back(p2);
        players.push_back(p3);
        players.push_back(p4);

        // initalize both the player scores to 0
        playerScores[0] = 0;
        playerScores[1] = 0;
        // player 0 can deal first. This is incremented mod 4 after every deal
        playerDealing = 0;
    }
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
    int getBidSuit();

    bool determineIfWonBidAndDeduct();
    int getTeamScore(int player);

    std::pair<int, bool> dealBidAndFullFiveTricks();

    // returns the cards the players played
    std::vector<Card> havePlayersPlayCards(int playerLeading);
    // have players play their cards and returns the player who won the trick
    std::pair<Card, int> havePlayersPlayCardsAndEvaluate(int playerLeading);

    int getHandSize(int playerNum) {
        return players[playerNum]->getSize();
    }
    int getNumPlayers() {
        return players.size();
    }
 private:
    Deck deck;
    // Array of pointers to an abstract class
    std::vector<Player*> players;
    std::vector<Card> discardDeck;
    std::vector<int> bidHistory;
    // only two player scores because there are two teams
    int playerScores[2];
    int bidAmount;
    int bidSuit;
    int bidder;
    int bidderInitialScore;
    int playerDealing;
    bool initalizedPlayersWithNew;
};
