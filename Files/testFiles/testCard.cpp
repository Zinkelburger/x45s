// Copyright Andrew Bernal 2023
#include "../gameState.hpp"
#include "../card.hpp"
#include "../suit.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CardTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(CardTestSuite)

// basic test of the card comparisons
BOOST_AUTO_TEST_CASE(testCardOperatorsTrumpSameSuitHearts) {
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::HEARTS);
    gameState->setSuitLed(Suit::SPADES);
    // 3 of hearts is bigger than 2 of hearts if trump is hearts
    Card c1(2, 1);
    Card c2(3, 1);
    BOOST_REQUIRE_EQUAL(c1 > c2, 0);
    BOOST_REQUIRE_EQUAL(c1 >= c2, 0);
    BOOST_REQUIRE_EQUAL(c1 < c2, 1);
    BOOST_REQUIRE_EQUAL(c1 <= c2, 1);
    BOOST_REQUIRE_EQUAL(c1 == c2, 0);
    BOOST_REQUIRE_EQUAL(c1 != c2, 1);
}

// confirms 3 of hearts is bigger than the king of every other suit when trump is hearts
BOOST_AUTO_TEST_CASE(testCardOperatorsTrumpDifferentSuitsHearts) {
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::HEARTS);
    gameState->setSuitLed(Suit::SPADES);
    // 3 of hearts is bigger than King of any other suit if trump is hearts (which it is)
    Card c1(13, 2);
    Card c2(3, 1);
    BOOST_REQUIRE_EQUAL(c1 > c2, 0);
    BOOST_REQUIRE_EQUAL(c1 >= c2, 0);
    BOOST_REQUIRE_EQUAL(c1 < c2, 1);
    BOOST_REQUIRE_EQUAL(c1 <= c2, 1);
    BOOST_REQUIRE_EQUAL(c1 == c2, 0);
    BOOST_REQUIRE_EQUAL(c1 != c2, 1);
    c1.setSuit(3);
    BOOST_REQUIRE_EQUAL(c1 > c2, 0);
    BOOST_REQUIRE_EQUAL(c1 >= c2, 0);
    BOOST_REQUIRE_EQUAL(c1 < c2, 1);
    BOOST_REQUIRE_EQUAL(c1 <= c2, 1);
    BOOST_REQUIRE_EQUAL(c1 == c2, 0);
    BOOST_REQUIRE_EQUAL(c1 != c2, 1);
    c1.setSuit(4);
    BOOST_REQUIRE_EQUAL(c1 > c2, 0);
    BOOST_REQUIRE_EQUAL(c1 >= c2, 0);
    BOOST_REQUIRE_EQUAL(c1 < c2, 1);
    BOOST_REQUIRE_EQUAL(c1 <= c2, 1);
    BOOST_REQUIRE_EQUAL(c1 == c2, 0);
    BOOST_REQUIRE_EQUAL(c1 != c2, 1);
}

// the same as the test case above. A small trump vs a big offsuit
BOOST_AUTO_TEST_CASE(testCardOperatorsTrumpDifferentSuitsClubs) {
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::CLUBS);
    // Suit Led shouldn't matter, I should make a case to test that...
    gameState->setSuitLed(Suit::DIAMONDS);

    // 3 of clubs is bigger than King of any other suit if trump is hearts (which it is)
    Card c1(13, 2);
    Card c2(3, 3);
    BOOST_REQUIRE_EQUAL(c1 > c2, 0);
    BOOST_REQUIRE_EQUAL(c1 >= c2, 0);
    BOOST_REQUIRE_EQUAL(c1 < c2, 1);
    BOOST_REQUIRE_EQUAL(c1 <= c2, 1);
    BOOST_REQUIRE_EQUAL(c1 == c2, 0);
    BOOST_REQUIRE_EQUAL(c1 != c2, 1);
    c1.setSuit(1);
    BOOST_REQUIRE_EQUAL(c1 > c2, 0);
    BOOST_REQUIRE_EQUAL(c1 >= c2, 0);
    BOOST_REQUIRE_EQUAL(c1 < c2, 1);
    BOOST_REQUIRE_EQUAL(c1 <= c2, 1);
    BOOST_REQUIRE_EQUAL(c1 == c2, 0);
    BOOST_REQUIRE_EQUAL(c1 != c2, 1);
    c1.setSuit(4);
    BOOST_REQUIRE_EQUAL(c1 > c2, 0);
    BOOST_REQUIRE_EQUAL(c1 >= c2, 0);
    BOOST_REQUIRE_EQUAL(c1 < c2, 1);
    BOOST_REQUIRE_EQUAL(c1 <= c2, 1);
    BOOST_REQUIRE_EQUAL(c1 == c2, 0);
    BOOST_REQUIRE_EQUAL(c1 != c2, 1);
}

// comparison operator for the ace of hearts vs not 5 & J of hearts
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingHearts) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::HEARTS);
    gameState->setSuitLed(Suit::HEARTS);

    for (int i = 2; i <= 13; i++) {
        // ignore the 5 and Jack for the test
        if (i == 5 || i == 11) continue;
        Card other(i, Suit::HEARTS);
        BOOST_ASSERT(aceOfHearts > other);
    }
}

// test the comparison operator for the ace of hearts vs the 5 & J of hearts
BOOST_AUTO_TEST_CASE(testAceOfHeartsVS5JHearts) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::HEARTS);
    gameState->setSuitLed(Suit::HEARTS);

    Card other1(5, Suit::HEARTS);
    BOOST_ASSERT(aceOfHearts < other1);

    Card other2(11, Suit::HEARTS);
    BOOST_ASSERT(aceOfHearts < other2);
}

// comparison operator for the ace of hearts vs not 5 & J of diamonds
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingDiamonds) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);

    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::DIAMONDS);
    gameState->setSuitLed(Suit::DIAMONDS);

    for (int i = 1; i <= 13; i++) {
        // ignore the 5 and Jack for the test
        if (i == 5 || i == 11) continue;
        Card other(i, Suit::DIAMONDS);
        BOOST_ASSERT(aceOfHearts > other);
    }
}

// test the comparison operator for the ace of hearts vs the 5 & J of diamonds
BOOST_AUTO_TEST_CASE(testAceOfHeartsVS5JDiamonds) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::DIAMONDS);
    gameState->setSuitLed(Suit::DIAMONDS);

    Card other1(5, Suit::DIAMONDS);
    BOOST_ASSERT(aceOfHearts < other1);

    Card other2(11, Suit::DIAMONDS);
    BOOST_ASSERT(aceOfHearts < other2);
}

// comparison operator for the ace of hearts vs not 5 & J of clubs
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingClubs) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::CLUBS);
    gameState->setSuitLed(Suit::CLUBS);

    for (int i = 1; i <= 13; i++) {
        // ignore the 5 and Jack for the test
        if (i == 5 || i == 11) continue;
        Card other(i, Suit::CLUBS);
        BOOST_ASSERT(aceOfHearts > other);
    }
}

// test the comparison operator for the ace of hearts vs the 5 & J of clubs
BOOST_AUTO_TEST_CASE(testAceOfHeartsVS5JClubs) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);

    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::CLUBS);
    gameState->setSuitLed(Suit::CLUBS);

    Card other1(5, Suit::CLUBS);
    BOOST_ASSERT(aceOfHearts < other1);

    Card other2(11, Suit::CLUBS);
    BOOST_ASSERT(aceOfHearts < other2);
}
// comparison operator for the ace of hearts vs not 5 & J of spades
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingSpades) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
    
    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::SPADES);
    gameState->setSuitLed(Suit::SPADES);

    for (int i = 1; i <= 13; i++) {
        // ignore the 5 and Jack for the test
        if (i == 5 || i == 11) continue;
        Card other(i, Suit::SPADES);
        BOOST_ASSERT(aceOfHearts > other);
    }
}

// test the comparison operator for the ace of hearts vs the 5 & J of spades
BOOST_AUTO_TEST_CASE(testAceOfHeartsVS5JSpades) {
    Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);

    GameState* gameState = GameState::getInstance();
    gameState->setTrump(Suit::SPADES);
    gameState->setSuitLed(Suit::SPADES);

    Card other1(5, Suit::SPADES);
    BOOST_ASSERT(aceOfHearts < other1);

    Card other2(11, Suit::SPADES);
    BOOST_ASSERT(aceOfHearts < other2);
}

BOOST_AUTO_TEST_CASE(DefaultConstructorTest) {
    Card card;
    BOOST_TEST(card.getValue() == -1000);
    BOOST_TEST(card.getSuit() == -1000);
}

BOOST_AUTO_TEST_CASE(ParameterizedConstructorTest) {
    Card card(1, Suit::DIAMONDS);
    BOOST_TEST(card.getValue() == 1);
    BOOST_TEST(card.getSuit() == Suit::DIAMONDS);
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest) {
    Card card1(1, Suit::DIAMONDS);
    Card card2(card1);
    BOOST_TEST(card2.getValue() == 1);
    BOOST_TEST(card2.getSuit() == Suit::DIAMONDS);
}

BOOST_AUTO_TEST_CASE(AssignmentOperatorTest) {
    Card card1(1, Suit::DIAMONDS);
    Card card2;
    card2 = card1;
    BOOST_TEST(card2.getValue() == 1);
    BOOST_TEST(card2.getSuit() == Suit::DIAMONDS);
}

BOOST_AUTO_TEST_CASE(OutputStreamOperatorTest1) {
    Card card(-1, Suit::ACE_OF_HEARTS);
    std::ostringstream oss;
    oss << card;
    BOOST_TEST(oss.str() == "Ace of Hearts, ");
}

BOOST_AUTO_TEST_CASE(OutputStreamOperatorTest2) {
    Card card(13, Suit::SPADES);
    std::ostringstream oss;
    oss << card;
    BOOST_TEST(oss.str() == "King of Spades, ");
}

BOOST_AUTO_TEST_CASE(GetValueTest) {
    Card card(1, Suit::HEARTS);
    BOOST_TEST(card.getValue() == 1);
}

BOOST_AUTO_TEST_CASE(GetSuitTest) {
    Card card(1, Suit::HEARTS);
    BOOST_TEST(card.getSuit() == Suit::HEARTS);
}

BOOST_AUTO_TEST_CASE(SetValueTest) {
    Card card;
    card.setValue(1);
    BOOST_TEST(card.getValue() == 1);
}

BOOST_AUTO_TEST_CASE(SetSuitTest) {
    Card card;
    card.setSuit(Suit::HEARTS);
    BOOST_TEST(card.getSuit() == Suit::HEARTS);
}

BOOST_AUTO_TEST_SUITE_END()
