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

    void setWalls(float wallWidth);
    Velocity getRandomVelocity(Velocity max);

public:
    vector<Ball *> balls;

    BallManager(Rectangle ballRec);
    ~BallManager();

    void addBallCenter();

    void updateBalls();
    void checkWallCollisions(Ball *ball);
    void checkBallCollisions(Ball *ball);

    inline vector<Rectangle> getWalls() { return walls; };
};