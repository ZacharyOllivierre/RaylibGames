#pragma once
#include <string>
#include <raylib.h>

using std::string;

struct BaseEntityData
{
    string name;
    float health;
    float speed;
    bool alive;
};

class Entity
{
protected:
    BaseEntityData baseData;

    Rectangle *simRec;

    Vector2 position;
    Vector2 newPosition;
    bool moving;

    void moveTowardsNewPos();

public:
    bool insideStructure;

public:
    Entity(Vector2 position, BaseEntityData baseData, Rectangle *simRec);
    ~Entity();

    void update();
    void moveToPos(Vector2 newPosition);
    void takeDamage(float damage);

    inline bool getAlive() { return baseData.alive; }
    inline float getHealth() { return baseData.health; }
    inline Vector2 *getPosition() { return &position; }
};