// Copyright Andrew Bernal 2023
#include <boost/test/unit_test.hpp>
#include "../deck.hpp"
#include "../player.hpp"
#include "../card.hpp"
#include "../suit.hpp"
#include "../45s.hpp"
#include <vector>
#include <algorithm>

class nonBidder : public Player {
public:
        void discard() override {}
        std::pair<int, Suit::Suit> getBid([[maybe_unused]] const std::vector<int>& bidHistory) override {
                return {0, Suit::SPADES};
        }
        Suit::Suit bagged() override {
                return Suit::SPADES;
        }
        Card playCard([[maybe_unused]] const std::vector<Card>& cardsPlayedThisHand) override {
                return hand.back();
        }
};

class bigBidder : public Player {
public:
        void discard() override {}
        std::pair<int, Suit::Suit> getBid([[maybe_unused]] const std::vector<int>& bidHistory) override {
                return {30, Suit::CLUBS};
        }
        Suit::Suit bagged() override {
                return Suit::CLUBS;
        }
        Card playCard([[maybe_unused]] const std::vector<Card>& cardsPlayedThisHand) override {
                return hand.back();
        }
};

// Test that the deal_players method deals 5 cards to each player
BOOST_AUTO_TEST_CASE(TestDealPlayers) {
        x45s game([]{return new nonBidder;}, []{return new nonBidder;},
                []{return new nonBidder;}, []{return new nonBidder;});
        game.deal_players();
        for (int i = 0; i < 4; i++) {
                BOOST_CHECK_EQUAL(game.getHandSize(i), 5);
        }
}

// Test that the default constructor creates an x45s object with 4 players
BOOST_AUTO_TEST_CASE(TestDefaultConstructor) {
        x45s game([]{return new nonBidder;}, []{return new nonBidder;},
                []{return new nonBidder;}, []{return new nonBidder;});
        BOOST_CHECK_EQUAL(game.getNumPlayers(), 4);
}

// Test that the constructor with player arguments creates an x45s object with the specified players
BOOST_AUTO_TEST_CASE(TestConstructorWithPlayerArguments) {
        nonBidder p1, p2, p3, p4;
        x45s game(&p1, &p2, &p3, &p4);
        BOOST_CHECK_EQUAL(game.getNumPlayers(), 4);
}

// Test that the reset method resets the hands of the players and the deck
BOOST_AUTO_TEST_CASE(TestReset) {
        x45s game([]{return new nonBidder;}, []{return new nonBidder;},
                []{return new nonBidder;}, []{return new nonBidder;});
        game.deal_players();
        game.reset();
        for (int i = 0; i < 4; i++) {
        BOOST_CHECK_EQUAL(game.getHandSize(i), 0);
        }
}

BOOST_AUTO_TEST_CASE(TestInitializationOfPlayers) {
        x45s game([]{return new nonBidder;}, []{return new nonBidder;},
                []{return new nonBidder;}, []{return new nonBidder;});
        // when the players are made, none of them should have any cards
        for (int i = 0; i < 4; i++) {
        BOOST_CHECK_EQUAL(game.getHandSize(i), 0);
        }
}

BOOST_AUTO_TEST_CASE(TestGetBidder) {
        x45s game([]{return new nonBidder;}, []{return new nonBidder;},
                []{return new bigBidder;}, []{return new nonBidder;});

        game.biddingPhase();
        // the bigBidder should win the bid. He is player 2, and bids 30 in clubs
        BOOST_CHECK_EQUAL(game.getBidder(), 2);
        BOOST_CHECK_EQUAL(game.getTrump(), Suit::CLUBS);
        BOOST_CHECK_EQUAL(game.getBidAmount(), 30);
}
