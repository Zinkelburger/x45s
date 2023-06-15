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
#include "suit.hpp"

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
// deals the kiddie to the player who won the bid.
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
// evaluates the cards thrown by all four players. Returns the winning card
// precondition: suit led and trump have been previously set
Card x45s::evaluate_trick(Card card1, Card card2, Card card3, Card card4) {
    // may be slower than like 15 if statements, but it is very readable
    std::vector<Card> c = {card1, card2, card3, card4};
    return *std::max_element(c.begin(), c.end());
}

// evaluate the trick thrown by all four players. Returns the winning card
Card x45s::evaluate_trick(std::vector<Card> c) {
    // may be slower than like 15 if statements, but it is very readable
    return *std::max_element(c.begin(), c.end());
}

// adds 5 points to the team input (team 0 or 1)
void x45s::updateScores(int player) {
    if (player != 0 && player != 1) {
        throw std::invalid_argument("Invalid player " + std::to_string(player) +
        " in updateScores. Must be 0 or 1");
    }
    playerScores[player] += 5;
}

// returns the score of the team input (team 0 or 1)
int x45s::getTeamScore(int player) {
    if (player != 0 && player != 1) {
        throw std::invalid_argument("Invalid player " +
        std::to_string(player) + " in updateScores. Must be 0 or 1");
    }
    return playerScores[player];
}

// returns true if either team has won
bool x45s::hasWon() {
    // if either team has 120 points or greater, then they have won
    if (playerScores[0] >= 120) {
        return true;
    }
    if (playerScores[1] >= 120) {
        return true;
    }
    return false;
}

// Returns the number of the player that has won the game (0 or 1).
// Returns -1 if no one has won
int x45s::whichPlayerWon() {
    if (playerScores[0] >= 120) {
            return 0;
    }
    if (playerScores[1] >= 120) {
        return 1;
    }
    return -1;
}

// calls each player's discard method
void x45s::havePlayersDiscard() {
    for (auto& e : players) {
        e->discard();
    }
}

int x45s::getBidAmount() {
    return bidAmount;
}

// returns the player who bid and if they won the bid or not
std::pair<int, bool> x45s::dealBidAndFullFiveTricks() {
    deal_players();

    int bidder = getBidder();
    setBid(bidAmount, bidder);

    trump = getBidSuit();

    deal_kiddie(bidder);
    havePlayersDiscard();

    deal_players();

    int firstPlayer = bidder;
    // stores the card and the player who played it
    std::pair<Card, int> highCard;

    // there are 5 tricks in each hand
    for (int i = 0; i < 5; i++) {
        std::pair<Card, int> winnerAndCard = havePlayersPlayCardsAndEvaluate(firstPlayer);
        // player who won will lead the next trick
        firstPlayer = winnerAndCard.second;

        if (winnerAndCard.first > highCard.first || i == 0) {
            highCard = winnerAndCard;
        }
    }
    // give the high card bonus to the team
    updateScores(highCard.second % 2);

 
    return std::make_pair(bidder, determineIfWonBidAndDeduct());
}

// sets both the bid amount and the player who bid
void x45s::setBid(int bid, int bidderNum) {
    bidAmount = bid;
    bidder = bidderNum;

    // keep track of the bidder's score to detect if they won their bid or not
    bidderInitialScore = playerScores[bidder];
}

// gets the bids for each player and increments the dealer
int x45s::getBidder() {
    // start the hand with a fresh bid history
    bidHistory.clear();
    // pair is <value, suit>
    std::pair<int, Suit::Suit> currentBid;
    // initalize maxBid to something small, so it is replaced immediately
    std::pair<int, Suit::Suit> maxBid = {INT32_MIN, Suit::ACE_OF_HEARTS};
    int firstPlayer = -1;

    // the player dealing bids last and can possiblly be bagged
    for (int i = playerDealing + 1; i < playerDealing + 4; i++) {
        currentBid = players[i % 4]->getBid(bidHistory);
        // .first is the value
        if (currentBid.first != 0) {
            bidHistory.push_back(currentBid.first);
            if (currentBid.first > maxBid.first) {
                maxBid = currentBid;
                firstPlayer = i;
            }
        }
    }

    // if no player bid, then bag the dealer
    if (maxBid.first <= 0) {
        currentBid.second = players[playerDealing]->bagged();
        firstPlayer = playerDealing;
    } else {
        currentBid = players[playerDealing]->getBid(bidHistory);
        // .first is the value
        if (currentBid.first != 0) {
            bidHistory.push_back(currentBid.first);
            if (currentBid.first > maxBid.first) {
                maxBid = currentBid;
                firstPlayer = playerDealing;
            }
        }
    }

    // increment the player dealing mod 4
    playerDealing++;
    playerDealing %= 4;

    bidAmount = maxBid.first;
    bidSuit = maxBid.second;

    return firstPlayer;
}

int x45s::getBidSuit() {
    return bidSuit;
}

bool x45s::determineIfWonBidAndDeduct() {
    // if (current score - the amount bid) >= to their score before they bid
    // then they made their bid
    if (playerScores[bidder] - bidAmount < bidderInitialScore) {
        playerScores[bidder] = bidderInitialScore - bidAmount;
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

// returns a vector of the cards played by each player
std::vector<Card> x45s::havePlayersPlayCards(int playerLeading) {
    std::vector<Card> cardsPlayed(4);
    cardsPlayed[playerLeading % 4] = (*(players[playerLeading % 4])).playCard(cardsPlayed);
    suitLed = cardsPlayed[playerLeading % 4].getSuit();
    if (suitLed == Suit::ACE_OF_HEARTS) {
        suitLed = Suit::HEARTS;
    }
    for (int cardNum = ++playerLeading; cardNum < 4 + playerLeading; cardNum++) {
       cardsPlayed[playerLeading % 4] = (*(players[cardNum % 4])).playCard(cardsPlayed);
    }
    return cardsPlayed;
}

// have players play their cards and returns the Card & Player who won the trick
std::pair<Card, int> x45s::havePlayersPlayCardsAndEvaluate(int playerLeading) {
    std::vector<Card> cardsPlayed(4);

    cardsPlayed[playerLeading % 4] = (*(players[playerLeading % 4])).playCard(cardsPlayed);
    suitLed = cardsPlayed[playerLeading % 4].getSuit();

    if (suitLed == Suit::ACE_OF_HEARTS) {
        suitLed = Suit::HEARTS;
    }
    // calls playCard for the other 3 players and stores their card in an array
    for (int cardNum = ++playerLeading; cardNum < 4 + playerLeading; cardNum++) {
        cardsPlayed[playerLeading % 4] = (*(players[cardNum % 4])).playCard(cardsPlayed);
    }

    Card winningCard(evaluate_trick(cardsPlayed));
    int winningPlayer;
    if (winningCard == cardsPlayed[0]) {
        winningPlayer = 0;
    } else if (winningCard == cardsPlayed[1]) {
        winningPlayer = 1;
    } else if (winningCard == cardsPlayed[2]) {
        winningPlayer = 2;
    } else {
        winningPlayer = 3;
    }
    return std::make_pair(winningCard, winningPlayer);
}

