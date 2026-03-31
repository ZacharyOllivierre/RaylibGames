#pragma once
#include "basicFunctions.h"
#include <cmath>

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

    Ball(Point p, Velocity maxSpeed, Velocity v = {0, 0}, int radius = 10);

    void update();

    void collision(WallSide side);
    void ballCollision(Ball *otherBall);
};