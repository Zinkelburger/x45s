// Copyright Andrew Bernal 2023
#pragma once
#include "45s.hpp"

class Human : public Player {
 public:
    Card takeCardInput(std::string inp) override;
    void discard() override;
    std::pair<int, int> getBid(const std::vector<int>& bidHistory) override;
    std::pair<int, int> bagged() override;
    Card playCard(std::vector<Card> cardsPlayedThisHand) override;
};