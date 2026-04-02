#include "ballManager.h"
#include <iostream>

BallManager::BallManager(Rectangle ballRec, const vector<Rectangle> &walls)
{
    this->ballRec = ballRec;
    this->walls = walls;

    srand(time(nullptr));

    // 7 is about ideal
    int screenSizeDivisior = 10;
    maxVelocity = {ballRec.width / screenSizeDivisior, ballRec.height / screenSizeDivisior};

    // Testing add default balls
    // int numBalls = 4;
    // for (int i = 0; i < numBalls; i++)
    // {
    //     addBallCenter();
    // }
}

BallManager::~BallManager()
{
    for (auto ball : balls)
    {
        delete ball;
    }
}

void BallManager::addBallCenter()
{
    Velocity startingVelocity = getRandomVelocity(maxVelocity);
    Point centerPoint = getCenter(ballRec);

    Ball *newBall = new Ball(
        centerPoint,
        maxVelocity,
        startingVelocity);

    balls.push_back(newBall);
}

void BallManager::updateBalls()
{
    for (Ball *ball : balls)
    {
        ball->update();

        checkWallCollisions(ball);
    }
}

void BallManager::checkWallCollisions(Ball *ball)
{
    if (walls.size() < 4)
    {
        return;
    }

    int ballLeft = ball->point.x - ball->radius;
    int ballRight = ball->point.x + ball->radius;
    int ballTop = ball->point.y - ball->radius;
    int ballBottom = ball->point.y + ball->radius;

    int wallLeft = walls[Left].x + walls[Left].width;
    int wallRight = walls[Right].x;
    int wallTop = walls[Top].y + walls[Top].height;
    int wallBottom = walls[Bottom].y;

    // Check for collisions or past collision
    if (ballLeft < wallLeft)
    {
        ball->collision(Left);
        ball->point.x = wallLeft + ball->radius;
    }
    else if (ballRight > wallRight)
    {
        ball->collision(Right);
        ball->point.x = wallRight - ball->radius;
    }
    else if (ballTop < wallTop)
    {
        ball->collision(Top);
        ball->point.y = wallTop + ball->radius;
    }
    else if (ballBottom > wallBottom)
    {
        ball->collision(Bottom);
        ball->point.y = wallBottom - ball->radius;
    }
}

Velocity BallManager::getRandomVelocity(Velocity max)
{
    float xV = rand() % (int)max.xVel;
    float yV = rand() % (int)max.yVel;

    if (rand() % 2 == 0)
    {
        xV *= -1;
    }
    if (rand() % 2 == 0)
    {
        yV *= -1;
    }

    return {xV, yV};
}