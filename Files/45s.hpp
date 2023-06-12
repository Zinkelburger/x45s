// Copyright Andrew Bernal 2023
#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include "trumpGlobalVariable.hpp"
#include "deck.hpp"
#include "player.hpp"

// start with an x because I can't start with a number
class x45s {
 public:
    // no default constructor, have to give it the players at initalization
    x45s() = delete;
    x45s(Player* p1, Player* p2, Player* p3, Player* p4) : deck() {
        initalizePlayers(p1, p2, p3, p4);
    }
    void deal_players();
    void shuffle();
    void reset();
    // deal the kiddie to the player who won the bid. (0-3)
    void deal_kiddie(int winner);
    // evaluate the trick thrown all four players. Returns the winning card
    Card evaluate_trick(Card card1, Card card2, Card card3, Card card4);

    // keep track of the scores for each player. If someone gets 120 points, they win and the game ends
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

    bool determineIfWonBid();
    int getTeamScore(int player);

    std::vector<Card> havePlayersPlayCards(int playerLeading);
private:
    void x45s::initalizePlayers(Player* p1, Player* p2, Player* p3, Player* p4);
    // I think it makes more sense for 45s to make a copy of the vector, and main can call methods for 45s to do things with the deck (like deal)
    Deck deck;
    // I can't have an array of an abstract class, so I have an array of pointers to an abstract class 
    std::vector<Player*> players;
    std::vector<Card> discardDeck;
    // only two player scores because there are two teams
    int playerScores[2];
    int bidAmount;
    int bidSuit;
    int bidder;
    int bidderInitialScore;
    int playerDealing;
};