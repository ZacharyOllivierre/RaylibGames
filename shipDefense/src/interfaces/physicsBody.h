#pragma once

#include "raylib.h"

class PhysicsBody
{
public:
    PhysicsBody(Rectangle body = {0, 0, 0, 0},
                Vector2 drag = {1.0f, 1.0f})
        : body(body), drag(drag) {}

    void update(float delta)
    {
        velocity.x += acceleration.x * delta;
        velocity.y += acceleration.y * delta;

        // drag (not realistic but better controls)
        velocity.x *= drag.x;
        velocity.y *= drag.y;

        body.x += velocity.x * delta;
        body.y += velocity.y * delta;

        // clear acceleration after each frame
        acceleration = {0, 0};
    }

    void addForce(Vector2 force)
    {
        acceleration.x += force.x;
        acceleration.y += force.y;
    }

    void setVelocity(Vector2 velocity)
    {
        this->velocity = velocity;
    }

    Vector2 getVelocity() const
    {
        return velocity;
    }

    Vector2 getPosition() const
    {
        return {body.x, body.y};
    }

    void setPosition(Vector2 position)
    {
        body.x = position.x;
        body.y = position.y;
    }

    Rectangle getBody() const
    {
        return body;
    }

private:
    Rectangle body{0, 0, 0, 0};
    Vector2 drag = {1.0f, 1.0f};

    Vector2 velocity{0, 0};
    Vector2 acceleration{0, 0};
};
