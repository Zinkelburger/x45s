// Copyright Andrew Bernal 2023
#include "../card.hpp"
#include "../suit.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CardTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(CardTestSuite)

// basic test of the card comparisons
BOOST_AUTO_TEST_CASE(testCardOperatorsTrumpSameSuitHearts) {
        // 3 of hearts is bigger than 2 of hearts if trump is hearts
        Card c1(2, Suit::HEARTS);
        Card c2(3, Suit::HEARTS);
        BOOST_REQUIRE(lessThan(c1, c2, Suit::SPADES, Suit::HEARTS));
        BOOST_REQUIRE(!lessThan(c2, c1, Suit::SPADES, Suit::HEARTS));
        BOOST_REQUIRE_EQUAL(c1 == c2, 0);
        BOOST_REQUIRE_EQUAL(c1 != c2, 1);
}

// confirms 3 of hearts is bigger than the king of every other suit when trump is hearts
BOOST_AUTO_TEST_CASE(testCardOperatorsTrumpDifferentSuitsHearts) {
        // 3 of hearts is bigger than King of any other suit if trump is hearts (which it is)
        Card c1(13, 2);
        Card c2(3, 1);
        BOOST_REQUIRE(lessThan(c1, c2, Suit::SPADES, Suit::HEARTS));

        c1.setSuit(3);
        BOOST_REQUIRE(lessThan(c1, c2, Suit::SPADES, Suit::HEARTS));

        c1.setSuit(4);
        BOOST_REQUIRE(lessThan(c1, c2, Suit::SPADES, Suit::HEARTS));
}

// the same as the test case above. A small trump vs a big offsuit
BOOST_AUTO_TEST_CASE(testCardOperatorsTrumpDifferentSuitsClubs) {
        // 3 of clubs is bigger than King of any other suit if trump is hearts (which it is)
        Card c1(13, 2);
        Card c2(3, 3);
        BOOST_REQUIRE(lessThan(c1, c2, Suit::DIAMONDS, Suit::CLUBS));

        c1.setSuit(1);
        BOOST_REQUIRE(lessThan(c1, c2, Suit::DIAMONDS, Suit::CLUBS));

        c1.setSuit(4);
        BOOST_REQUIRE(lessThan(c1, c2, Suit::DIAMONDS, Suit::CLUBS));
}

// comparison operator for the ace of hearts vs all hearts
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingHearts) {
        Card aceOfHearts(1, Suit::HEARTS);

        for (int i = 2; i <= 13; i++) {
                // ignore the 5 and Jack
                if (i == 5 || i == 11) continue;
                Card other(i, Suit::HEARTS);
                BOOST_ASSERT(lessThan(other, aceOfHearts, Suit::HEARTS, Suit::HEARTS));
                BOOST_ASSERT(!lessThan(aceOfHearts, other, Suit::HEARTS, Suit::HEARTS));
        }

        // against 5 and J
        BOOST_ASSERT(!lessThan(Card(5, 1), aceOfHearts, Suit::HEARTS, Suit::HEARTS));
        BOOST_ASSERT(!lessThan(Card(11, 1), aceOfHearts, Suit::HEARTS, Suit::HEARTS));

        BOOST_ASSERT(lessThan(aceOfHearts, Card(5, 1), Suit::HEARTS, Suit::HEARTS));
        BOOST_ASSERT(lessThan(aceOfHearts, Card(11, 1), Suit::HEARTS, Suit::HEARTS));
}

// comparison operator for the ace of hearts all diamonds when the suit is diamonds
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingDiamonds) {
        Card aceOfHearts(1, Suit::HEARTS);

        for (int i = 1; i <= 13; i++) {
                // ignore the 5 and Jack
                if (i == 5 || i == 11) continue;
                Card other(i, Suit::DIAMONDS);
                BOOST_ASSERT(lessThan(other, aceOfHearts, Suit::DIAMONDS, Suit::DIAMONDS));
                BOOST_ASSERT(!lessThan(aceOfHearts, other, Suit::DIAMONDS, Suit::DIAMONDS));
        }

        // against 5 and J
        BOOST_ASSERT(!lessThan(Card(5, Suit::DIAMONDS), aceOfHearts, Suit::DIAMONDS, Suit::DIAMONDS));
        BOOST_ASSERT(!lessThan(Card(11, Suit::DIAMONDS), aceOfHearts, Suit::DIAMONDS, Suit::DIAMONDS));

        BOOST_ASSERT(lessThan(aceOfHearts, Card(5, Suit::DIAMONDS), Suit::DIAMONDS, Suit::DIAMONDS));
        BOOST_ASSERT(lessThan(aceOfHearts, Card(11, Suit::DIAMONDS), Suit::DIAMONDS, Suit::DIAMONDS));
}

// comparison operator for the ace of hearts vs not 5 & J of clubs
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingClubs) {
        Card aceOfHearts(1, Suit::HEARTS);

        for (int i = 1; i <= 13; i++) {
                // ignore the 5 and Jack
                if (i == 5 || i == 11) continue;
                Card other(i, Suit::CLUBS);
                BOOST_ASSERT(lessThan(other, aceOfHearts, Suit::CLUBS, Suit::CLUBS));
                BOOST_ASSERT(!lessThan(aceOfHearts, other, Suit::CLUBS, Suit::CLUBS));
        }

        // against 5 and J
        BOOST_ASSERT(!lessThan(Card(5, Suit::CLUBS), aceOfHearts, Suit::CLUBS, Suit::CLUBS));
        BOOST_ASSERT(!lessThan(Card(11, Suit::CLUBS), aceOfHearts, Suit::CLUBS, Suit::CLUBS));

        BOOST_ASSERT(lessThan(aceOfHearts, Card(5, Suit::CLUBS), Suit::CLUBS, Suit::CLUBS));
        BOOST_ASSERT(lessThan(aceOfHearts, Card(11, Suit::CLUBS), Suit::CLUBS, Suit::CLUBS));
}

// comparison operator for the ace of hearts vs not 5 & J of spades
BOOST_AUTO_TEST_CASE(testAceOfHeartsVSEverythingSpades) {
        Card aceOfHearts(1, Suit::HEARTS);

        for (int i = 1; i <= 13; i++) {
                // ignore the 5 and Jack
                if (i == 5 || i == 11) continue;
                Card other(i, Suit::SPADES);
                BOOST_ASSERT(lessThan(other, aceOfHearts, Suit::SPADES, Suit::SPADES));
                BOOST_ASSERT(!lessThan(aceOfHearts, other, Suit::SPADES, Suit::SPADES));
        }

        // against 5 and J
        BOOST_ASSERT(!lessThan(Card(5, Suit::SPADES), aceOfHearts, Suit::SPADES, Suit::SPADES));
        BOOST_ASSERT(!lessThan(Card(11, Suit::SPADES), aceOfHearts, Suit::SPADES, Suit::SPADES));

        BOOST_ASSERT(lessThan(aceOfHearts, Card(5, Suit::SPADES), Suit::SPADES, Suit::SPADES));
        BOOST_ASSERT(lessThan(aceOfHearts, Card(11, Suit::SPADES), Suit::SPADES, Suit::SPADES));
}

// 5 of spades beats all spades except itself. Suitled is spades, trump is spades
BOOST_AUTO_TEST_CASE(fiveAlwaysWinsSpadesSuitledSpades) {
        Suit::Suit trump = Suit::SPADES;
        Suit::Suit suitLed = Suit::SPADES;
        Card five(5, trump);

        for (int i = 1; i <= 13; i++) {
                // ignore the 5
                if (i == 5) continue;
                Card other(i, trump);
                BOOST_ASSERT(lessThan(other, five, suitLed, trump));
                BOOST_ASSERT(!lessThan(five, other, suitLed, trump));
        }

        // against itself
        BOOST_ASSERT(!lessThan(Card(5, trump), five, suitLed, trump));
        BOOST_ASSERT(!lessThan(five, Card(5, trump), suitLed, trump));
}

// 5 of spades beats all spades except itself. Suitled is hearts, trump is spades
BOOST_AUTO_TEST_CASE(fiveAlwaysWinsSpadesSuitledHearts) {
        Suit::Suit trump = Suit::SPADES;
        Suit::Suit suitLed = Suit::HEARTS;
        Card five(5, trump);

        for (int i = 1; i <= 13; i++) {
                // ignore the 5
                if (i == 5) continue;
                Card other(i, trump);
                BOOST_ASSERT(lessThan(other, five, suitLed, trump));
                BOOST_ASSERT(!lessThan(five, other, suitLed, trump));
        }

        // against itself
        BOOST_ASSERT(!lessThan(Card(5, trump), five, suitLed, trump));
        BOOST_ASSERT(!lessThan(five, Card(5, trump), suitLed, trump));
}

// J of spades beats all spades except itself and the 5. Suitled is spades, trump is spades
BOOST_AUTO_TEST_CASE(jackWinsSpadesSuitledSpades) {
        Suit::Suit trump = Suit::SPADES;
        Suit::Suit suitLed = Suit::SPADES;
        Card jack(11, trump);

        for (int i = 1; i <= 13; i++) {
                // ignore the 5
                if (i == 5 || i == 11) continue;
                Card other(i, trump);
                BOOST_ASSERT(lessThan(other, jack, suitLed, trump));
                BOOST_ASSERT(!lessThan(jack, other, suitLed, trump));
        }

        // against the 5
        BOOST_ASSERT(!lessThan(Card(5, trump), jack, suitLed, trump));
        BOOST_ASSERT(lessThan(jack, Card(5, trump), suitLed, trump));

        // against itself
        BOOST_ASSERT(!lessThan(jack, Card(11, trump), suitLed, trump));
        BOOST_ASSERT(!lessThan(Card(11, trump), jack, suitLed, trump));
}

// J of spades beats all spades except itself and the 5. Suitled is diamonds, trump is spades
BOOST_AUTO_TEST_CASE(jackWinsSpadesSuitledDiamonds) {
        Suit::Suit trump = Suit::SPADES;
        Suit::Suit suitLed = Suit::DIAMONDS;
        Card jack(11, trump);

        for (int i = 1; i <= 13; i++) {
                // ignore the 5
                if (i == 5 || i == 11) continue;
                Card other(i, trump);
                BOOST_ASSERT(lessThan(other, jack, suitLed, trump));
                BOOST_ASSERT(!lessThan(jack, other, suitLed, trump));
        }

        // against the 5
        BOOST_ASSERT(!lessThan(Card(5, trump), jack, suitLed, trump));
        BOOST_ASSERT(lessThan(jack, Card(5, trump), suitLed, trump));

        // against itself
        BOOST_ASSERT(!lessThan(Card(11, trump), jack, suitLed, trump));
        BOOST_ASSERT(!lessThan(jack, Card(11, trump), suitLed, trump));
}

BOOST_AUTO_TEST_CASE(IdenticalCardsOffsuite) {
        Card c1(10, Suit::DIAMONDS);
        Card c2(10, Suit::DIAMONDS);
        BOOST_REQUIRE_EQUAL(lessThan(c1, c2, Suit::HEARTS, Suit::CLUBS), false);
        BOOST_REQUIRE_EQUAL(lessThan(c2, c1, Suit::HEARTS, Suit::CLUBS), false);
        BOOST_REQUIRE_EQUAL(c1 == c2, true);
        BOOST_REQUIRE_EQUAL(c1 != c2, false);
}

BOOST_AUTO_TEST_CASE(IdenticalCardsTrump) {
        Card c1(10, Suit::DIAMONDS);
        Card c2(10, Suit::DIAMONDS);
        BOOST_REQUIRE_EQUAL(lessThan(c1, c2, Suit::HEARTS, Suit::DIAMONDS), false);
        BOOST_REQUIRE_EQUAL(lessThan(c2, c1, Suit::HEARTS, Suit::DIAMONDS), false);
        BOOST_REQUIRE_EQUAL(c1 == c2, true);
        BOOST_REQUIRE_EQUAL(c1 != c2, false);
}

BOOST_AUTO_TEST_CASE(IdenticalCardsSuitled) {
        Card c1(10, Suit::DIAMONDS);
        Card c2(10, Suit::DIAMONDS);
        BOOST_REQUIRE_EQUAL(lessThan(c1, c2, Suit::DIAMONDS, Suit::CLUBS), false);
        BOOST_REQUIRE_EQUAL(lessThan(c2, c1, Suit::DIAMONDS, Suit::CLUBS), false);
        BOOST_REQUIRE_EQUAL(c1 == c2, true);
        BOOST_REQUIRE_EQUAL(c1 != c2, false);
}

BOOST_AUTO_TEST_CASE(DefaultConstructorTest) {
        Card card;
        BOOST_TEST(card.getValue() == -1000);
        BOOST_TEST(card.getSuit() == Suit::INVALID);
}

BOOST_AUTO_TEST_CASE(ParameterizedConstructorTest) {
        Card card(1, Suit::DIAMONDS);
        BOOST_TEST(card.getValue() == 1);
        BOOST_TEST(card.getSuit() == Suit::DIAMONDS);
}

BOOST_AUTO_TEST_CASE(AceOfHeartsConstructorTest) {
        Card card(1, Suit::HEARTS);
        BOOST_TEST(card.getValue() == 0xACE);
        BOOST_TEST(card.getSuit() == Suit::HEARTS);
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
        Card card(1, Suit::HEARTS);
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
        BOOST_TEST(card.getValue() == 0xACE);
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
