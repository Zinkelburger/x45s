// Copyright Andrew Bernal 2023
#include <boost/test/unit_test.hpp>
#include "../deck.hpp"
#include "../card.hpp"
#include "../suit.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

BOOST_AUTO_TEST_SUITE(DeckTestSuite)

BOOST_AUTO_TEST_CASE(ConstructorTest) {
        Deck deck;
        BOOST_TEST(deck.getSize() == 52);
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest) {
        Deck deck1;
        Deck deck2(deck1);
        BOOST_TEST(deck2.getSize() == 52);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest) {
        Deck deck1;
        Deck deck2(std::move(deck1));
        BOOST_TEST(deck2.getSize() == 52);
}

BOOST_AUTO_TEST_CASE(CopyAssignmentTest) {
        Deck deck1;
        Deck deck2;
        deck2 = deck1;
        BOOST_TEST(deck2.getSize() == 52);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentTest) {
        Deck deck1;
        Deck deck2;
        deck2 = std::move(deck1);
        BOOST_TEST(deck2.getSize() == 52);
}

BOOST_AUTO_TEST_CASE(ShuffleTest) {
        Deck deck;
        Card c = deck.peek_back();
        deck.shuffle();
        BOOST_TEST(deck.peek_back() != c);
}

BOOST_AUTO_TEST_CASE(PopBackTest) {
        Deck deck;
        deck.pop_back();
        BOOST_TEST(deck.getSize() == 51);
}

BOOST_AUTO_TEST_CASE(PeekBackTest) {
        Deck deck;
        deck.peek_back();
        BOOST_TEST(deck.getSize() == 52);
}

BOOST_AUTO_TEST_CASE(PushBackTest) {
        Deck deck;
        Card card{1, 1};
        deck.push_back(card);
        BOOST_TEST(deck.getSize() == 53);
}

BOOST_AUTO_TEST_CASE(ResetTest) {
        Deck deck;
        std::vector<Card> pack = deck.getPack();
        deck.reset();
        BOOST_TEST(std::equal(pack.begin(), pack.end(), deck.getPack().begin()));
}

BOOST_AUTO_TEST_CASE(RemoveCardAceOfHearts) {
        Deck deck;
        Card card(1, 1);
        deck.removeCard(card);
        BOOST_TEST(deck.getSize() == 51);
}

BOOST_AUTO_TEST_CASE(FindCardTest) {
        Deck deck;
        Card card{1, 2};
        bool found = deck.containsCard(card.getValue(), card.getSuit());
        BOOST_TEST(found);
}

// test that all cards are in the deck before a shuffle operation
BOOST_AUTO_TEST_CASE(beforeShuffleTestAll52CardsExist) {
        Deck deck{};

        // ace of hearts is the first case to check
        BOOST_REQUIRE(deck.containsCard(0xACE, Suit::HEARTS));

        // hearts is special as j starts at 2
        int i = Suit::HEARTS;
        for (int j = 2; j < 14; j++) {
                BOOST_REQUIRE(deck.containsCard(j, i));
        }

        for (i = Suit::DIAMONDS; i <= Suit::SPADES; i++) {
                for (int j = 1; j < 14; j++) {
                        BOOST_REQUIRE(deck.containsCard(j, i));
                }
        }
}

// test that all cards are in the deck after a shuffle operation
BOOST_AUTO_TEST_CASE(shuffleTestAll52CardsExist) {
        Deck deck{};
        deck.shuffle();

        // ace of hearts is the first case to check
        BOOST_REQUIRE(deck.containsCard(0xACE, Suit::HEARTS));

        // hearts is special
        int i = Suit::HEARTS;
        for (int j = 2; j < 14; j++) {
                BOOST_REQUIRE(deck.containsCard(j, i));
        }

        for (i = Suit::DIAMONDS; i <= Suit::SPADES; i++) {
                for (int j = 1; j < 14; j++) {
                        BOOST_REQUIRE(deck.containsCard(j, i));
                }
        }
}

// test that all cards are in the deck after ten shuffle operations
BOOST_AUTO_TEST_CASE(manyShufflesTestAll52CardsExist) {
        Deck deck{};
        for (int i = 0; i < 100; i++) {
                deck.shuffle();
        }

        // hearts is special
        int i = Suit::HEARTS;
        for (int j = 2; j < 14; j++) {
                BOOST_REQUIRE(deck.containsCard(j, i));
        }

        for (i = Suit::DIAMONDS; i <= Suit::SPADES; i++) {
                for (int j = 1; j < 14; j++) {
                        BOOST_REQUIRE(deck.containsCard(j, i));
                }
        }

        BOOST_REQUIRE(deck.containsCard(0xACE, Suit::HEARTS));
}

BOOST_AUTO_TEST_SUITE_END()
