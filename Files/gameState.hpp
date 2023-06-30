// Copyright Andrew Bernal 2023
#pragma once
// Removed global variables and replaced with a singleton class
// x45s is the only one who should change the singleton class
class GameState {
 private:
    int trump;
    int suitLed;
    bool trumpInitalized;
    bool suitLedInitalized;
    static GameState* instance;

    GameState() : trump(0), suitLed(0), trumpInitalized(false), suitLedInitalized(false) {}

 public:
    static GameState* getInstance() {
        if (instance == nullptr) {
            instance = new GameState();
        }
        return instance;
    }

    void setTrump(int trump) {
        this->trump = trump;
        trumpInitalized = true;
    }

    int getTrump() const {
        return trump;
    }

    void setSuitLed(int suitLed) {
        this->suitLed = suitLed;
        suitLedInitalized = true;
    }

    int getSuitLed() const {
        return suitLed;
    }

    bool getTrumpInitalized() {
        return trumpInitalized;
    }

    bool getSuitLedInitalized() {
        return suitLedInitalized;
    }

    void unsetTrumpInitalized() {
        trumpInitalized = false;
    }

    void unsetSuitLedInitalized() {
        suitLedInitalized = false;
    }
};