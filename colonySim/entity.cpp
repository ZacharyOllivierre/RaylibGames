#include "entity.h"

Entity::Entity(Vector2 position, string name, float health, float speed, bool alive)
{
    this->position = position;
    this->name = name;
    this->health = health;
    this->speed = speed;
    this->alive = alive;
    alive = true;
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
    this->newPosition = newPosition;
    moving = true;
}

// Private
void Entity::moveTowardsNewPos()
{
    if (!moving)
    {
        return;
    }

    // Move closer in the y
    if (position.y != newPosition.y)
    {
        if (position.y < newPosition.y)
        {
            position.y += speed;
        }
        else
        {
            position.y -= speed;
        }
    }
    // If inline in y move closer in x
    if (position.x != newPosition.x)
    {
        if (position.x < newPosition.x)
        {
            position.x += speed;
        }
        else
        {
            position.x -= speed;
        }
    }
    else
    {
        moving = false;
    }
}