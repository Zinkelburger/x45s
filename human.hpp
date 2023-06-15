// Copyright Andrew Bernal 2023
#pragma once
#include "x45s.hpp"

// you can use namespace x45s, but this makes the parts of the library more explicit
class Human : public x45s::Player {
 public:
    void discard() override;
    std::pair<int, int> getBid(const std::vector<int>& bidHistory) override;
    std::pair<int, int> bagged() override;
    x45s::Card playCard(std::vector<x45s::Card> cardsPlayedThisHand) override;
};