// Copyright Andrew Bernal 2023
#include "human.hpp"
#include <string>
#include <iostream>
#include <iterator>
#include "x45s.hpp"

void Human::discard() {

}

// returned pair is bidAmount, suit
std::pair<int, int> Human::getBid(const std::vector<int>& bidHistory) {
    std::cout << "Currently your bid\nCurrent bids: ";
    std::copy(bidHistory.begin(), bidHistory.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "\nYour hand: "; 
    printHand(std::cout);

    int bidAmount;
    std::cout << "\nEnter 0, 15, 20, 25, or 30\n";
    do {
        std::cout << "Bid Amount: ";
        std::cin >> bidAmount;
    } while (bidAmount % 5 != 0 || bidAmount < 15 || bidAmount > 30);

    if (bidAmount != 0) {
        std::cout << "\nEnter H, D, S, C (e.g. H for Hearts)\n";
        char c;
        do {
            std::cout << "\nSuit: ";
            std::cin >> c;
        } while (!(c == 'H' || c == 'D' || c == 'S' || c == 'C'));

        int suit;
        switch (c) {
            case 'H':
                suit = Suit::HEARTS;
                break;
            case 'D':
                suit = Suit::DIAMONDS;
                break;
            case 'S':
                suit = Suit::SPADES;
                break;
            case 'C':
                suit = Suit::CLUBS;
                break;
            default:
                throw("How did you get here\n");
        }
        return std::make_pair(bidAmount, suit);
    } else {
        return std::make_pair(0, 0);
    }
}

Suit::Suit Human::bagged() {
    std::cout << "You have been bagged! You must bid\n";
    std::cout << "\nEnter H, D, S, C (e.g. H for Hearts)\n";
        char c;
        do {
            std::cout << "\nSuit: ";
            std::cin >> c;
        } while (!(c == 'H' || c == 'D' || c == 'S' || c == 'C'));

        int suit;
        switch (c) {
            case 'H':
                suit = Suit::HEARTS;
                break;
            case 'D':
                suit = Suit::DIAMONDS;
                break;
            case 'S':
                suit = Suit::SPADES;
                break;
            case 'C':
                suit = Suit::CLUBS;
                break;
            default:
                throw("How did you get here\n");
        }
}

x45s::Card Human::playCard(std::vector<x45s::Card> cardsPlayedThisHand) {

}