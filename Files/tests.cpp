// Copyright Andrew Bernal 2023
#include "trumpGlobalVariable.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "45s.hpp"
#include "suit.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

// have to set trump and suitLed. trump is hearts, suitled is spades
int trump = Suit::HEARTS;
int suitLed = Suit::SPADES;

// basic test of the card comparisons
BOOST_AUTO_TEST_CASE(testCardOperatorsTrumpSameSuitHearts) {
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
  trump = Suit::CLUBS;
  // Suit Led shouldn't matter, I should make a case to test that...
  suitLed = Suit::DIAMONDS;
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

// test that all cards are in the deck before a shuffle operation
BOOST_AUTO_TEST_CASE(beforeShuffleTestAll52CardsExist) {
  Deck deck{};

  // ace of hearts is the first case to check, as it is -1, ACE_OF_HEARTS
  BOOST_REQUIRE(deck.containsCard(-1, Suit::ACE_OF_HEARTS));

  // hearts is special as j starts at 2. I could use an if-statement, or do this...
  int i = 1;
  for (int j = 2; j < 14; j++) {
      BOOST_REQUIRE(deck.containsCard(j, i));
  }

  for (int i = Suit::DIAMONDS; i <= Suit::SPADES; i++) {
    for (int j = 1; j < 14; j++) {
      BOOST_REQUIRE(deck.containsCard(j, i));
    }
  }
}

// test that all cards are in the deck after a shuffle operation
BOOST_AUTO_TEST_CASE(shuffleTestAll52CardsExist) {
  Deck deck{};
  deck.shuffle();

  // ace of hearts is the first case to check, as it is -1, ACE_OF_HEARTS
  BOOST_REQUIRE(deck.containsCard(-1, Suit::ACE_OF_HEARTS));

  // hearts is special
  int i = 1;
  for (int j = 2; j < 14; j++) {
      BOOST_REQUIRE(deck.containsCard(j, i));
  }

  for (int i = Suit::DIAMONDS; i <= Suit::SPADES; i++) {
    for (int j = 1; j < 14; j++) {
      BOOST_REQUIRE(deck.containsCard(j, i));
    }
  }
}

// test that all cards are in the deck after ten shuffle operations
BOOST_AUTO_TEST_CASE(manyShufflesTestAll52CardsExist) {
  Deck deck{};
  for (int i = 0; i < 10; i++) {
    deck.shuffle();
  }

  // hearts is special
  int i = 1;
  for (int j = 2; j < 14; j++) {
      BOOST_REQUIRE(deck.containsCard(j, i));
  }

  for (int i = Suit::DIAMONDS; i <= Suit::SPADES; i++) {
    for (int j = 1; j < 14; j++) {
      BOOST_REQUIRE(deck.containsCard(j, i));
    }
  }
}

// comparison operator for the ace of hearts vs not 5 & J of hearts
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingHearts) {
  Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  trump = Suit::HEARTS;
  suitLed = Suit::HEARTS;
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
  trump = Suit::HEARTS;
  suitLed = Suit::HEARTS;

  Card other1(5, Suit::HEARTS);
  BOOST_ASSERT(aceOfHearts < other1);

  Card other2(11, Suit::HEARTS);
  BOOST_ASSERT(aceOfHearts < other2);
}

// comparison operator for the ace of hearts vs not 5 & J of diamonds
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingDiamonds) {
  Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  trump = Suit::DIAMONDS;
  suitLed = Suit::DIAMONDS;
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
  trump = Suit::DIAMONDS;
  suitLed = Suit::DIAMONDS;

  Card other1(5, Suit::DIAMONDS);
  BOOST_ASSERT(aceOfHearts < other1);

  Card other2(11, Suit::DIAMONDS);
  BOOST_ASSERT(aceOfHearts < other2);
}

// comparison operator for the ace of hearts vs not 5 & J of clubs
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingClubs) {
  Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  trump = Suit::CLUBS;
  suitLed = Suit::CLUBS;
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
  trump = Suit::CLUBS;
  suitLed = Suit::CLUBS;

  Card other1(5, Suit::CLUBS);
  BOOST_ASSERT(aceOfHearts < other1);

  Card other2(11, Suit::CLUBS);
  BOOST_ASSERT(aceOfHearts < other2);
}
// comparison operator for the ace of hearts vs not 5 & J of spades
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingSpades) {
  Card aceOfHearts(-1, Suit::ACE_OF_HEARTS);
  trump = Suit::SPADES;
  suitLed = Suit::SPADES;
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
  trump = Suit::SPADES;
  suitLed = Suit::SPADES;

  Card other1(5, Suit::SPADES);
  BOOST_ASSERT(aceOfHearts < other1);

  Card other2(11, Suit::SPADES);
  BOOST_ASSERT(aceOfHearts < other2);
}
