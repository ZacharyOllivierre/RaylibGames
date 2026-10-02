#pragma once

#include <algorithm>
#include <vector>

#include "interfaces/collidable.h"

class CollisionManager
{
public:
    void add(Collidable *object)
    {
        if (object == nullptr ||
            std::find(objects.begin(), objects.end(), object) != objects.end())
            return;

        objects.push_back(object);
    }

    void checkCollisions()
    {
        for (int first = 0; first < objects.size(); first++)
        {
            for (int second = first + 1; second < objects.size(); second++)
            {
                Collidable *firstObject = objects[first];
                Collidable *secondObject = objects[second];

                if (CheckCollisionRecs(firstObject->getCollisionBounds(),
                                       secondObject->getCollisionBounds()))
                {
                    Rectangle firstBounds = firstObject->getCollisionBounds();
                    Rectangle secondBounds = secondObject->getCollisionBounds();
                    CollisionContact firstContact = getContact(firstBounds, secondBounds);

                    CollisionContact secondContact = {
                        {-firstContact.normal.x, -firstContact.normal.y},
                        firstContact.penetration};

                    firstObject->onCollision(*secondObject, firstContact);
                    secondObject->onCollision(*firstObject, secondContact);
                }
            }
        }
    }

    void remove(Collidable *object)
    {
        objects.erase(
            std::remove(objects.begin(), objects.end(), object),
            objects.end());
    }

private:
    static CollisionContact getContact(Rectangle first,
                                       Rectangle second)
    {
        float overlapX = std::min(first.x + first.width,
                                  second.x + second.width) -
                         std::max(first.x, second.x);
        float overlapY = std::min(first.y + first.height,
                                  second.y + second.height) -
                         std::max(first.y, second.y);

        if (overlapX < overlapY)
        {
            float direction = (first.x + first.width / 2.0f <
                               second.x + second.width / 2.0f)
                                  ? -1.0f
                                  : 1.0f;
            return {{direction, 0.0f}, overlapX};
        }

        float direction = (first.y + first.height / 2.0f <
                           second.y + second.height / 2.0f)
                              ? -1.0f
                              : 1.0f;
        return {{0.0f, direction}, overlapY};
    }

    std::vector<Collidable *> objects;
};