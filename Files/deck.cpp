// Copyright Andrew Bernal 2023
#include "deck.hpp"
#include <ctime>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include "suit.hpp"

Deck::Deck() {
        // initalize 52 cards, Hearts = 1, Diamonds = 2, Clubs = 3, Spades = 4, Invalid = 5
        for (int i = Suit::HEARTS; i < Suit::INVALID; i++) {
                for (int j = 1; j < 14; j++) {
                        pack.push_back(Card(j, i));
                }
        }
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
        for (unsigned i = 0; i < d.pack.size(); i++) {
                out << d.pack[i];
                out << " ";
        }
        return out;
}
