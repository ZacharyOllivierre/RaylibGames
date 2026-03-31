#pragma once
#include "ball.h"
#include "basicFunctions.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include "raylib.h"

using std::vector;

class BallManager
{
private:
    Rectangle ballRec;
    vector<Rectangle> walls;

    Velocity maxVelocity;

    Velocity getRandomVelocity(Velocity max);

public:
    vector<Ball *> balls;

    BallManager(Rectangle ballRec, const vector<Rectangle> &walls);
    ~BallManager();

    void addBallCenter();

    void updateBalls();
    void checkWallCollisions(Ball *ball);
};