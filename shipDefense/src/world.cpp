#include "world.h"

#include "interfaces/physicsBody.h"
#include "raymath.h"

#include <algorithm>

World::World(Vector2 dimensions)
    : dimensions(dimensions)
{
}

void World::constrain(PhysicsBody &body)
{
    Rectangle bodyRect = body.getBody();

    float minX = 0.0f;
    float minY = 0.0f;

    float maxX = std::max(0.0f, dimensions.x - bodyRect.width);
    float maxY = std::max(0.0f, dimensions.y - bodyRect.height);

    Vector2 position = body.getPosition();

    bool hitX = false;
    bool hitY = false;

    if (position.x < minX)
    {
        position.x = minX;
        hitX = true;
    }
    else if (position.x > maxX)
    {
        position.x = maxX;
        hitX = true;
    }

    if (position.y < minY)
    {
        position.y = minY;
        hitY = true;
    }
    else if (position.y > maxY)
    {
        position.y = maxY;
        hitY = true;
    }

    body.setPosition(position);

    // zero velocity in hit direction
    Vector2 velocity = body.getVelocity();
    if (hitX)
        velocity.x = 0;

    if (hitY)
        velocity.y = 0;

    body.setVelocity(velocity);
}

Vector2 World::getDimensions() const
{
    return dimensions;
}

Rectangle World::getBounds() const
{
    return {
        0,
        0,
        dimensions.x,
        dimensions.y};
}
