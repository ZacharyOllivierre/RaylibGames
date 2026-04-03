#pragma once
#include "basicFunctions.h"
#include <cmath>
#include <vector>

using std::vector;

// Update to acceleration
struct Velocity
{
    float xVel;
    float yVel;
};

enum WallSide
{
    Left = 0,
    Top = 1,
    Right = 2,
    Bottom = 3
};

class Ball
{
private:
    Velocity velocity;

    Velocity maxVelocity;

    void updateColor();

public:
    int radius;
    Point point;
    RGB ballColor;

    /*Used so that points only given on first instance of
    collision (rolling). Follows wallSide enum*/
    vector<bool> collisionTracker;

    Ball(Point p, Velocity ms, Velocity v = {0, 0}, int r = 15);

    void update(float gravity);

    void collision(WallSide side, float bc, float f);

    void addVelocity(Velocity v);
};