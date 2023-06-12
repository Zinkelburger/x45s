// Copyright Andrew Bernal 2023
#include "45s.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <vector>
#include "trumpGlobalVariable.hpp"
#include "deck.hpp"
#include "player.hpp"
#include "human.hpp"
#include "computer.hpp"

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
    // pair is <value, suit>
    std::pair<int, int> currentBid;
    std::pair<int, int> maxBid = {-2, -2};
    int i = 0;
    int firstPlayer = -1;
    for (auto& e : players) {
        currentBid = e->getBid();
        if (currentBid.first > maxBid.first) {
            maxBid = currentBid;
            firstPlayer = i;
        }
        i++;
    }

    // if no player bid, then bag the dealer
    if (maxBid.first <= 0) {
        what = players[playerDealing]->getBid();
        firstPlayer = playerDealing;
        
    }
    playerDealing++;
    playerDealing %= 4;

    bidAmount = maxBid.first;
    bidSuit = maxBid.second;

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
