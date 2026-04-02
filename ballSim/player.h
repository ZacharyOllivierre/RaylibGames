#pragma once

class Player
{
private:
    int points;

public:
    Player();

    void addPoints(int points = 1);
    void subtractPoints(int points);

    inline int getPoints() { return points; }
};