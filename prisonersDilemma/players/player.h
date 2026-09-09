#pragma once

#include "../gameTypes.h"

#include <string>
#include <vector>

class Player
{
public:
    Player(const std::string &name) : name(name), score(0) {}

    virtual Move chooseMove(const std::vector<Round> &gameHistory) = 0;
    virtual void reset() { score = 0; }

    void addScore(const int &score) { this->score += score; }

    const int &getScore() const { return score; }
    const std::string &getName() const { return name; }

protected:
    std::string name;
    int score;
};