// Copyright Andrew Bernal 2023
#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include "deck.hpp"
#include "card.hpp"
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
    Card evaluate_trick(const Card& card1, const Card& card2, const Card& card3, const Card& card4);
    Card evaluate_trick(std::vector<Card> c);

    // Increments the team's score by 5 (team is either 0 or 1)
    void updateScores(int team);

    bool hasWon();
    // Returns the number of the player that has won the game (from 0 to 3).
    // Returns -1 if no one has won
    int whichTeamWon();
    // calls the player discard method for each player
    void havePlayersDiscard();
    // a getter, not the other thing
    int getBidAmount();

    // Has each player bid & returns the player who won the bid (0, 1, 2, 3)
    void biddingPhase();
    // returns the trump
    Suit::Suit getTrump();

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
