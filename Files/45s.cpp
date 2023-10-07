// Copyright Andrew Bernal 2023
#include "45s.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <vector>
#include "deck.hpp"
#include "player.hpp"
#include "suit.hpp"
#include "card.hpp"

// pass it players that are already initalized
x45s::x45s(Player* p1, Player* p2, Player* p3, Player* p4) : deck() {
    initalizedPlayersWithNew = false;
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    players.push_back(p4);

    // initalize both the teams' scores to 0
    teamScores[0] = 0;
    teamScores[1] = 0;

    teamScoresThisHand[0] = 0;
    teamScoresThisHand[1] = 0;

    // player 0 can deal first
    playerDealing = 0;
}

// pass it constructors, with new and stuff
x45s::x45s(std::function<Player*()> cp1, std::function<Player*()> cp2,
    std::function<Player*()> cp3, std::function<Player*()> cp4) : deck() {
    initalizedPlayersWithNew = true;
    players.push_back(cp1());
    players.push_back(cp2());
    players.push_back(cp3());
    players.push_back(cp4());

    // initalize both the player scores to 0
    teamScores[0] = 0;
    teamScores[1] = 0;

    teamScoresThisHand[0] = 0;
    teamScoresThisHand[1] = 0;

    // player 0 can deal first. This is incremented mod 4 after every deal
    playerDealing = 0;
}

// shuffles the deck 10 times
void x45s::shuffle() {
    deck.shuffle(10);
}

// deals players until each has 5 cards
void x45s::deal_players() {
    // make sure each player is dealt until their hand is 5 cards
    for (unsigned i = 0; i < players.size(); i++) {
        while (players[i]->getSize() < 5) {
            Card c = deck.pop_back();
            players[i]->dealCard(c);
        }
    }
}

// deals the kiddie (3 cards) to the player who won the bid.
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
Card x45s::evaluate_trick(const Card& card1, const Card& card2, const Card& card3, const Card& card4) {
    std::vector<Card> c = {card1, card2, card3, card4};
    // may be slower than like 15 if statements, but it is very readable
    return *std::max_element(c.begin(), c.end(), [this](const Card& lhs, const Card& rhs) {
        return lessThan(lhs, rhs, suitLed, trump);
    });
}

// evaluates the cards thrown by all four players. Returns the winning card
Card x45s::evaluate_trick(std::vector<Card> c) {
    // may be slower than like 15 if statements, but it is very readable
    return *std::max_element(c.begin(), c.end(), [this](const Card& lhs, const Card& rhs) {
        return lessThan(lhs, rhs, suitLed, trump);
    });
}

// Increments the team's score by 5 (team is either 0 or 1)
void x45s::updateScores(int team) {
    if (team != 0 && team != 1) {
        throw std::invalid_argument("Invalid player " + std::to_string(team) +
        " in updateScores. Must be 0 or 1");
    }
    teamScores[team] += 5;
}

// returns the score of the team input (team 0 or 1)
int x45s::getTeamScore(int team) {
    if (team != 0 && team != 1) {
        throw std::invalid_argument("Invalid player " +
        std::to_string(team) + " in updateScores. Must be 0 or 1");
    }
    return teamScores[team];
}

// returns true if either team has won
bool x45s::hasWon() {
    // if either team has 120 points or greater, then they have won
    return teamScores[0] >= 120 || teamScores[1] >= 120;
}

// Returns the number of the team that won the game (0 or 1).
// Returns -1 if no one has won
int x45s::whichTeamWon() {
    return (teamScores[0] >= 120) ? 0 : (teamScores[1] >= 120) ? 1 : -1;
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
    // initial deal
    deal_players();

    // have players bid
    biddingPhase();

    // deal the kiddie to the player who won the bid
    deal_kiddie(bidder);

    // let the players discard. Then deal them more cards
    havePlayersDiscard();
    deal_players();

    int firstPlayer = bidder;
    // stores the card and the player who played it
    std::pair<Card, int> highCard;

    // 5 tricks in each hand
    for (int i = 0; i < 5; i++) {
        std::pair<Card, int> winnerAndCard = havePlayersPlayCardsAndEvaluate(firstPlayer);
        // player who won will lead the next trick
        firstPlayer = winnerAndCard.second;

        if (i == 0 || lessThan(highCard.first, winnerAndCard.first, suitLed, trump)) {
            highCard = winnerAndCard;
        }
    }
    // give the team with the high card their bonus
    updateScores(highCard.second % 2);

    return {bidder, deductAfterBid()};
}

// gets the bids for each player and increments the dealer
void x45s::biddingPhase() {
    // start the hand with a fresh bid history
    bidHistory.clear();

    // bid is <value, suit>
    std::pair<int, Suit::Suit> currentBid;
    // initalize maxBid to something small, so it is replaced immediately
    std::pair<int, Suit::Suit> maxBid = {INT32_MIN, Suit::INVALID};
    int playerWinningBid = -1;

    // the dealing player bids last, and can possiblly be bagged
    for (int i = playerDealing + 1; i < playerDealing + 4; i++) {
        // get the player's bid. Pass them the bidHistory
        currentBid = players[i % 4]->getBid(bidHistory);
        // save the bid history
        bidHistory.push_back(currentBid.first);
        if (currentBid.first > maxBid.first) {
            // save the bid value, suit
            maxBid = currentBid;
            // save the player # who bid the most
            playerWinningBid = i % 4;
        }
    }

    // dealer's bid. Either bagged or normal
    if (maxBid.first <= 0) {
        // dealer is bagged
        // player bids 15, gets to pick the suit
        currentBid = {15, players[playerDealing]->bagged()};
        playerWinningBid = playerDealing;
    // otherwise the dealer bids like normal
    } else {
        currentBid = players[playerDealing]->getBid(bidHistory);
        // .first is the value
        if (currentBid.first != 0) {
            bidHistory.push_back(currentBid.first);
            if (currentBid.first > maxBid.first) {
                maxBid = currentBid;
                playerWinningBid = playerDealing;
            }
        }
    }

    bidAmount = maxBid.first;
    trump = maxBid.second;

    // increment the player dealing mod 4
    playerDealing++;
    playerDealing %= 4;

    bidder = playerWinningBid;
}

Suit::Suit x45s::getTrump() {
    return trump;
}

bool x45s::deductAfterBid() {
    bool wonBid = true;
    // opposing team always gets their score added
    teamScores[bidder + 1 % 2] += teamScoresThisHand[bidder + 1 % 2];

    // deduct if the bidder did not make their bid
    if (teamScoresThisHand[bidder % 2] < bidAmount) {
        teamScores[bidder % 2] -= bidAmount;
        wonBid = false;
    }

    return wonBid;
}

void x45s::reset() {
    for (auto& e : players) {
        e->resetHand();
    }
    deck.reset();
    teamScoresThisHand[0] = 0;
    teamScoresThisHand[1] = 0;
}

// returns a vector of the cards played by each player
std::vector<Card> x45s::havePlayersPlayCards(int playerLeading) {
    std::vector<Card> cardsPlayed(4);

    // first player, so we can get suitLed
    cardsPlayed[playerLeading % 4] = players[playerLeading % 4]->playCard(cardsPlayed);

    suitLed = cardsPlayed[playerLeading % 4].getSuit();

    for (int cardNum = ++playerLeading; cardNum < 4 + playerLeading; cardNum++) {
       cardsPlayed[playerLeading % 4] = players[cardNum % 4]->playCard(cardsPlayed);
    }
    return cardsPlayed;
}

// have players play their cards, returns the Card & Player who won the trick
std::pair<Card, int> x45s::havePlayersPlayCardsAndEvaluate(int playerLeading) {
    std::vector<Card> cardsPlayed(4);

    cardsPlayed[playerLeading % 4] = players[playerLeading % 4]->playCard(cardsPlayed);
    
    suitLed = cardsPlayed[playerLeading % 4].getSuit();

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
    return {winningCard, winningPlayer};
}

