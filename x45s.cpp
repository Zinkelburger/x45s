// Copyright Andrew Bernal 2023
#include "x45s.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

int suitLed = INT32_MIN;
int trump = INT32_MIN;

namespace x45s {
    using std::to_string;
    using std::vector;
    using std::ostream;
    using std::istream;

x45s::x45s(Player* p1, Player* p2, Player* p3, Player* p4) : deck() {
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

x45s::x45s(std::function<Player*()> cp1, std::function<Player*()> cp2,
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

    int firstPlayer = bidder + 1;
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

Suit::Suit x45s::getBidSuit() {
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
        suitLed = trump;
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
        suitLed = trump;
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

    // 45s Card is different from a normal card because of the 5 of hearts and the rules
    // These cards are programmed to follow 45s rules for <, >, ==, etc.

    // Ace of Hearts is set to value: -1, suit: ACE_OF_HEARTS

    // suits are 1, 2, 3, 4
    // 1 = hearts, 2 = diamonds, 3 = clubs, 4 = spades
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
        if (suitLed == INT32_MIN) {
            throw std::invalid_argument("Suit Led is not initalized!");
        }
        // neither card is suitLed. They will both lose
        // and there is no way to directly compare them.
        if (lhs.getSuit() != suitLed && rhs.getSuit() != suitLed) {
            return -1;
        }

        // now we know one of the cards is of suitLed
        int heartsAndDiamonds[14] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int clubsAndSpades[14] = {13, 12, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int* order;
        if (suitLed == Suit::HEARTS || suitLed == Suit::DIAMONDS) {
            order = heartsAndDiamonds;
        } else {
            order = clubsAndSpades;
        }

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

    // returns true if a card is higher than another card, considering the trump
    bool operator<(const Card& lhs, const Card& rhs) {
        // all of the suits, in order from highest to lowest
        if (trump == INT32_MIN) {
            throw std::invalid_argument("Trump is not initalized!");
        }

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
            // TODO(zinkelburger): Find a better way to resolve "neither card is a winner" situation
            if (r == -1) {
                return false;
            }
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

    // a comparison function that does not use global variables
    // so you can pass it the suitLed and Trump without changing them globally
    bool Card::lessThan(const Card& other, int inpSuit, int inpTrump) {
        if (!(inpTrump == 0 || inpTrump == 1 || inpTrump == 2 || inpTrump == 3)) {
            throw std::invalid_argument("inpTrump is not valid!");
        }
        // all of the suits, in order from highest to lowest

        // hearts has a -10 at the end so all the arrays can be the same length
        // -1 represents the ace of hearts
        int hearts[14] = {5, 11, -1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2, -10};
        int diamonds[14] = {5, 11, -1, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
        int clubsAndSpades[14] = {5, 11, -1, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};

        // order is set to whatever array we need above
        int* order;

        // neither card is trump
        if ((other.getSuit() != inpTrump && other.getSuit() != Suit::ACE_OF_HEARTS)
        && (this->getSuit() != inpTrump && this->getSuit() != Suit::ACE_OF_HEARTS)) {
            int r = this->evaluateOffSuit(other, inpSuit);
            // TODO(zinkelburger): Find a better way to resolve "neither card is a winner" situation
            if (r == -1) {
                return false;
            }
            return r;
        }

        // otherwise comparison is easy, just loop through the order until the highest card is found
        if (inpTrump == Suit::HEARTS) {
            order = hearts;
        } else if (inpTrump == Suit::DIAMONDS) {
            order = diamonds;
        } else {
            // compiler wants to make sure order is initalized, so I put it in an else
            order = clubsAndSpades;
        }

        // we know at least one of the cards is trump by this point
        // both cards are trump
        if ((other.getSuit() == inpTrump || other.getSuit() == Suit::ACE_OF_HEARTS)
        && (this->getSuit() == inpTrump || this->getSuit() == Suit::ACE_OF_HEARTS)) {
            for (int i = 0; i < 14; i++) {
                if (this->getValue() == order[i]) {
                    return true;
                } else if (other.getValue() == order[i]) {
                    return false;
                }
            }
        // right side is trump and left is not
        } else if ((this->getSuit() == inpTrump || this->getSuit() == Suit::ACE_OF_HEARTS)
        && (other.getSuit() != inpTrump && other.getSuit() != Suit::ACE_OF_HEARTS)) {
            return true;
        // left side is trump and right is not
        } else if ((this->getSuit() != inpTrump && this->getSuit() != Suit::ACE_OF_HEARTS)
        && (other.getSuit() == inpTrump || other.getSuit() == Suit::ACE_OF_HEARTS)) {
            return false;
        }
        throw("How the hell did you get here");
    }

    // overloaded, this one takes a inpSuit
    // Returns 1 (true) if the left card is smaller than the right card, and 0 if false
    int Card::evaluateOffSuit(const Card& other, int inpSuit) {
        if (!(inpSuit == 0 || inpSuit == 1 || inpSuit == 2 || inpSuit == 3)) {
            throw std::invalid_argument("suitLed is not valid!");
        }

        // neither card is inpSuit. They will both lose, and there is no way to directly compare them.
        if (other.getSuit() != inpSuit && this->getSuit() != inpSuit) {
            return -1;
        }

        // now we know one of the cards is of inpSuit
        int heartsAndDiamonds[14] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int clubsAndSpades[14] = {13, 12, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int* order;
        if (inpSuit == Suit::HEARTS || inpSuit == Suit::DIAMONDS) {
            order = heartsAndDiamonds;
        } else {
            order = clubsAndSpades;
        }

        // both cards are inpSuit
        if (other.getSuit() == inpSuit && this->getSuit() == inpSuit) {
            for (int i = 0; i < 14; i++) {
                if (this->getValue() == order[i]) {
                    return true;
                } else if (other.getValue() == order[i]) {
                    return false;
                }
            }
        // right side is inpSuit and left is not
        } else if (this->getSuit() == inpSuit && other.getSuit() != inpSuit) {
            return true;
        // left side is inpSuit and right is not
        } else if (this->getSuit() != inpSuit && other.getSuit() == inpSuit) {
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
        // I think std::move is overkill as a Card is cheap to copy.
        // Probably the compiler optimizes it anyways...
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
        for (int i = Suit::HEARTS; i <= Suit::SPADES; i++) {
            for (int j = 1; j < 14; j++) {
                pack.push_back(Card(j, i));
            }
        }
        // set the ace of hearts to its special suit
        // fortunately this is the first card made (1,1)
        pack[0].setSuit(Suit::ACE_OF_HEARTS);
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

    std::ostream& operator<<(std::ostream& out, const Deck& d) {
        for (int i = 0; i < 52; i++) {
            out << d;
            out << " ";
        }
        return out;
    }
}  // namespace x45s
