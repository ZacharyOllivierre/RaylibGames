#include "ball.h"
#include <iostream>

Ball::Ball(Point p, Velocity ms, Velocity v, int r)
{
    point = p;
    maxVelocity = ms;
    velocity = v;
    radius = r;

    collisionTracker = {
        false,
        false,
        false,
        false,
    };

    updateColor();
}

void Ball::update(float gravity)
{
    velocity.yVel += gravity;

    // clamp max speed
    velocity.xVel = std::clamp(velocity.xVel, -maxVelocity.xVel, maxVelocity.xVel);
    velocity.yVel = std::clamp(velocity.yVel, -maxVelocity.yVel, maxVelocity.yVel);

    // Update position by velocity
    point.x += velocity.xVel;
    point.y += velocity.yVel;

    // Update color
    updateColor();
}

void Ball::collision(WallSide side, float bc, float f)
{
    // Apply friction in x on floor and ceiling
    switch (side)
    {
    case Top:
    case Bottom:
        velocity.yVel *= (-1 * bc);
        velocity.xVel *= f;
        break;

    case Left:
    case Right:
        velocity.xVel *= (-1 * bc);
        break;
    }
}

void Ball::addVelocity(Velocity v)
{
    v.xVel = abs(v.xVel);
    v.yVel = abs(v.yVel);

    velocity.xVel > 0 ? velocity.xVel += v.xVel : velocity.xVel -= v.xVel;
    velocity.yVel -= v.yVel;
}

void Ball::updateColor()
{
    float vel = fmax(abs(velocity.xVel), abs(velocity.yVel));
    float percentMax = vel / ((abs(maxVelocity.xVel) + abs(maxVelocity.yVel)) / 2);

    unsigned char r = static_cast<unsigned char>(255 * percentMax);
    unsigned char g = static_cast<unsigned char>(255 * (1.0f - percentMax));

    ballColor = {r, g, 0, 255};
}