#include "ballManager.h"
#include <iostream>

BallManager::BallManager(Rectangle ballRec)
{
    this->ballRec = ballRec;

    int wallWidth = 10;
    setWalls(wallWidth);

    srand(time(nullptr));

    maxVelocity = {ballRec.width / 7, ballRec.height / 7};

    // Testing add default balls
    int numBalls = 4;
    for (int i = 0; i < numBalls; i++)
    {
        addBallCenter();
    }
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
    Velocity maxStartVel = {30, 30};
    Velocity startingVelocity = getRandomVelocity(maxStartVel);
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

        // not working
        checkBallCollisions(ball);
    }
}

void BallManager::checkWallCollisions(Ball *ball)
{
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

void BallManager::checkBallCollisions(Ball *ball)
{
    for (Ball *b : balls)
    {
        if (b != ball)
        {
            // Not working
            bool collision = CheckCollisionCircles({ball->point.x, ball->point.y}, ball->radius, {b->point.x, b->point.y}, b->radius);
            std::cout << "Collision result " << collision << std::endl;
            if (collision)
            {
                // ball->ballCollision(b);
                std::cout << "Ball collision" << std::endl;
            }
        }
    }
}

void BallManager::setWalls(float wallWidth)
{
    Rectangle leftWall = {
        ballRec.x,
        ballRec.y,
        wallWidth,
        ballRec.height};

    Rectangle topWall = {
        ballRec.x,
        ballRec.y,
        ballRec.width,
        wallWidth};

    Rectangle rightWall = {
        ballRec.x + ballRec.width - wallWidth,
        ballRec.y,
        wallWidth,
        ballRec.height};

    Rectangle bottomWall = {
        ballRec.x,
        ballRec.y + ballRec.height - wallWidth,
        ballRec.width,
        wallWidth};

    walls.push_back(leftWall);
    walls.push_back(topWall);
    walls.push_back(rightWall);
    walls.push_back(bottomWall);
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