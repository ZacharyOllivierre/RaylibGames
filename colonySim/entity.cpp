#include "entity.h"

Entity::Entity(Vector2 position, BaseEntityData baseData, Rectangle *simRec)
{
    this->position = position;
    this->baseData = baseData;
    this->simRec = simRec;
}

Entity::~Entity()
{
}

void Entity::update()
{
    if (moving)
    {
        moveTowardsNewPos();
    }
}

void Entity::moveToPos(Vector2 newPosition)
{
    // If valid position update newPosition and start moving
    if (CheckCollisionPointRec(newPosition, *simRec))
    {
        this->newPosition = newPosition;
        moving = true;
    }
}

void Entity::takeDamage(float damage)
{
    baseData.health -= damage;

    if (baseData.health <= 0)
    {
        baseData.alive = false;
    }
}

// Private
void Entity::moveTowardsNewPos()
{
    if (!moving)
    {
        return;
    }
    bool movingInY = false;
    bool movingInX = false;

    // Move closer in the y
    if (position.y != newPosition.y)
    {
        /* If current position is less than or greater than new position
        move speed amount towards position
        If moving speed amount shoots over desired distance(n),
        move n amount instead*/
        float yDist = abs(newPosition.y - position.y);
        float entitySpeedY = baseData.speed;

        if (yDist < baseData.speed)
        {
            entitySpeedY = yDist;
        }

        // move entitySpeed closer towards newPosition
        if (position.y < newPosition.y)
        {
            position.y += entitySpeedY;
        }
        else
        {
            position.y -= entitySpeedY;
        }
        movingInY = true;
    }
    // If inline in y move closer in x
    if (position.x != newPosition.x)
    {
        float xDist = abs(newPosition.x - position.x);
        float entitySpeedX = baseData.speed;

        /*To prevent extra speed on diagonal movement half base speed
        in x if already moving in y
        temp fix - make both x and y half if moving in both directions*/
        if (movingInY)
        {
            entitySpeedX /= 2;
        }

        if (xDist < baseData.speed)
        {
            entitySpeedX = xDist;
        }

        if (position.x < newPosition.x)
        {
            position.x += entitySpeedX;
        }
        else
        {
            position.x -= entitySpeedX;
        }
        movingInX = true;
    }

    /* If not moving in either direction set moving to false
    turns off pathing towards newPosition*/
    if (!movingInX && !movingInY)
    {
        moving = false;
    }
}