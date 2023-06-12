#pragma once
// Copyright Andrew Bernal 2023
#include <algorithm>
#include <string>
#include <vector>

namespace x45s {
    // start with an x because I can't start with a number
    class x45s {   
    public:
        x45s();
        void deal_players();
        void shuffle();
        void reset();
        // ask for bids
        // deal the kiddie to the player who won the bid. Need to input the number of the player who won the bid
        void deal_kiddie(int winner);
        // evaluate the trick thrown all four players. Returns the winning card
        Card evaluate_trick(Card card1, Card card2, Card card3, Card card4);
    
        // keep track of the scores for each player. If someone gets 120 points, they win and the game ends
        void updateScores(int player);

        bool hasWon();
        // Returns the number of the player that has won the game (from 0 to 3).
        // Returns -1 if no one has won
        int whichPlayerWon();
        void havePlayersDiscard();
        int getBidAmount();
        // sets both the bid amount and the player who bid
        void setBid(int bid, int bidderNum);
        // returns the player who won the bid (0, 1, 2, 3)
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

    // 45s Card is different from a normal card because of the 5 of hearts and the rules
    // These cards are programmed to follow 45s rules for <, >, ==, etc.

    // Could store 1 card in 1 char to save space, each only needs values 1-13 and 0-4
    // That would be an evil monstrosity, but if I need to save space, this is where to look
    class Card{
        int value;
        int suit;

    public:
        // Defined input, output, copy assignment, getters, and setters
        // Comparison operators require knowledge of the current trump suit

        // I have a default constructor so I can make pairs of cards
        Card() : value(-1000), suit(-1000) {}
        // value, suit
        Card(int inpValue, int inpSuit) : value(inpValue), suit(inpSuit) {}
        Card(const Card& c);
        friend std::ostream& operator<<(std::ostream& out, const Card& c);
        friend std::istream& operator>>(std::istream& in, Card& c);
        Card& operator=(const Card& inp) {
            value = inp.value;
            suit = inp.suit;
            return *this;
        }
        int getValue() const {
            return value;
        }
        int getSuit() const {
            return suit;
        }
        static std::string getSuitString();
        void setSuit(int inpSuit) {
            suit = inpSuit;
        }
        void setValue(int inpValue) {
            value = inpValue;
        }
    };
    // inspired from https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
    // only operator< is not inline because it is a large function
    bool operator<(const Card& lhs, const Card& rhs);
    inline bool operator==(const Card& lhs, const Card& rhs) {return lhs.getSuit() == rhs.getSuit()
    && rhs.getValue() == lhs.getValue();}
    inline bool operator!=(const Card& lhs, const Card& rhs) {return !operator==(lhs, rhs);}
    inline bool operator> (const Card& lhs, const Card& rhs) {return  operator<(rhs, lhs);}
    inline bool operator<=(const Card& lhs, const Card& rhs) {return !operator>(lhs, rhs);}
    inline bool operator>=(const Card& lhs, const Card& rhs) {return !operator<(lhs, rhs);}

    // store it in a vector so there are easy shuffle and remove operations
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
        Deck& operator=(const Deck& other);
        friend std::ostream& operator<< (std::ostream& out, const Deck& d);
    };

    // TODO(zinkelburger): make each player sf::drawable
    // player is designed to be overriden by Computer and Human
    //make each player sf::drawable
    // player is designed to be overriden by Computer and Human
    class Player{
     protected:
        std::vector<Card> hand;
     public:
        virtual Card takeCardInput(std::string inp) = 0;
        Player() {}
        // add the card to the player's hand
        void dealCard(Card c) {
            // Card is passed by value so I can simply do this
            hand.push_back(c);
        }
        virtual void discard() = 0;
        // bidAmount, suit
        virtual std::pair<int, int> getBid() = 0;
        virtual Card playCard(std::vector<Card> cardsPlayedThisHand) = 0;
        int getSize(){
            return hand.size();
        }
        void resetHand(){
            hand.clear();
        }
        void printHand(){
            for (auto& c : hand) {
                std::cout << c;
            }
            std::cout << "\n";
        }
        std::ostream& printHand(std::ostream& out){
            for (auto& c : hand) {
                out << c << " ";
            }
            out << "\n";
            return out;
        }
    };

    // Copyright Andrew Bernal 2023
    namespace Suit {
        enum Suit {
            ACE_OF_HEARTS,
            HEARTS,
            DIAMONDS,
            CLUBS,
            SPADES
        };
    }

    // Copyright Andrew Bernal 2023
    // https://learn.microsoft.com/en-us/cpp/cpp/extern-cpp?view=msvc-170
    // Global variables may be bad, but trump is shared across all of the files
    // main is the only one who should change trump and suitLed
    extern int trump;
    extern int suitLed;
}
