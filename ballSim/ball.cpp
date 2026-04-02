#include "ball.h"

Ball::Ball(Point p, Velocity maxSpeed, Velocity v, int radius)
{
    point = p;
    velocity = v;
    this->radius = radius;

    maxVelocity = maxSpeed;

    updateColor();
}

void Ball::update()
{
    // clamp max speed
    if (velocity.xVel > maxVelocity.xVel)
    {
        velocity.xVel = maxVelocity.xVel;
    }
    if (velocity.yVel > maxVelocity.yVel)
    {
        velocity.yVel = maxVelocity.yVel;
    }

    // Update position by velocity
    point.x += velocity.xVel;
    point.y += velocity.yVel;

    // Update color
    updateColor();
}

void Ball::collision(WallSide side)
{
    switch (side)
    {
    case Left:
        velocity.xVel *= -1;
        break;

    case Top:
        velocity.yVel *= -1;
        break;

    case Right:
        velocity.xVel *= -1;
        break;

    case Bottom:
        velocity.yVel *= -1;
        break;
    }
}

void Ball::updateColor()
{
    float vel = fmax(abs(velocity.xVel), abs(velocity.yVel));
    float percentMax = vel / ((abs(maxVelocity.xVel) + abs(maxVelocity.yVel)) / 2);

    unsigned char r = static_cast<unsigned char>(255 * percentMax);
    unsigned char g = static_cast<unsigned char>(255 * (1.0f - percentMax));

    ballColor = {r, g, 0, 255};
}