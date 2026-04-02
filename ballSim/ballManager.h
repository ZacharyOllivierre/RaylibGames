#pragma once
#include "ball.h"
#include "basicFunctions.h"
#include "player.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include "raylib.h"

using std::vector;

class BallManager
{
private:
    Player *player;

    Rectangle ballRec;
    vector<Rectangle> walls;

    Velocity maxVelocity;
    float bounceCoefficient;
    float gravity;
    float friction;

    Velocity getRandomVelocity(Velocity max);

public:
    vector<Ball *> balls;

    BallManager(Rectangle ballRec, const vector<Rectangle> &walls, Player *player);
    ~BallManager();

    void addBallCenter();

    void updateBalls();
    void checkWallCollisions(Ball *ball);
};