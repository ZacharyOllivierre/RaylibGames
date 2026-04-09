#include "ballManager.h"

BallManager::BallManager(Rectangle ballRec, const vector<Rectangle> &walls, Player *player)
{
    this->ballRec = ballRec;
    this->walls = walls;
    this->player = player;

    srand(time(nullptr));

    // 7 is about ideal
    int screenSizeDivisior = 7;
    maxVelocity = {ballRec.width / screenSizeDivisior, ballRec.height / screenSizeDivisior};

    // set shop stats
    bounceCoefficient = 0.85;
    friction = 0.85;
    gravity = 2;
    joltPercent = 0.1;

    // add starting ball
    addBallCenter();
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
    Velocity startingVelocity = getRandomVelocity({maxVelocity.xVel * joltPercent,
                                                   maxVelocity.yVel * joltPercent});
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
        ball->update(gravity);

        checkWallCollisions(ball);
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

    // make this a for loop, repeatable instead of seperate if else
    // check for collisions
    if (ballLeft <= wallLeft)
    {
        // collide and rebound
        ball->collision(Left, bounceCoefficient, friction);
        ball->point.x = wallLeft + ball->radius;

        // if collision has not happened on last frame add points
        if (!ball->collisionTracker[Left])
        {
            // add points for collision
            player->addPoints();

            // set current collision to collision type
            ball->collisionTracker[Left] = true;
        }
    }
    else
    {
        ball->collisionTracker[Left] = false;
    }

    if (ballRight >= wallRight)
    {
        ball->collision(Right, bounceCoefficient, friction);
        ball->point.x = wallRight - ball->radius;

        if (!ball->collisionTracker[Right])
        {
            player->addPoints();
            ball->collisionTracker[Right] = true;
        }
    }
    else
    {
        ball->collisionTracker[Right] = false;
    }

    if (ballTop <= wallTop)
    {
        ball->collision(Top, bounceCoefficient, friction);
        ball->point.y = wallTop + ball->radius;

        if (!ball->collisionTracker[Top])
        {
            player->addPoints();
            ball->collisionTracker[Top] = true;
        }
    }
    else
    {
        ball->collisionTracker[Top] = false;
    }

    if (ballBottom >= wallBottom)
    {
        ball->collision(Bottom, bounceCoefficient, friction);
        ball->point.y = wallBottom - ball->radius;

        if (!ball->collisionTracker[Bottom])
        {
            player->addPoints();

            ball->collisionTracker[Bottom] = true;
        }
    }
    else
    {
        ball->collisionTracker[Bottom] = false;
    }
}

void BallManager::joltBalls()
{
    for (Ball *ball : balls)
    {
        Velocity v = getRandomVelocity({maxVelocity.xVel * joltPercent,
                                        maxVelocity.yVel * joltPercent});

        ball->addVelocity(v);
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