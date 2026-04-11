#pragma once
#include <string>
#include <raylib.h>

using std::string;

class Entity
{
protected:
    string name;
    float health;
    float speed;
    bool alive;

    Rectangle *simRec;
    Vector2 position;
    Vector2 newPosition;
    bool moving;

    void moveTowardsNewPos();

public:
    Entity(Vector2 position, string name, float health, float speed, Rectangle *simRec, bool alive);
    ~Entity();

    void update();
    void moveToPos(Vector2 newPosition);
    void takeDamage(float damage);

    inline bool getAlive() { return alive; }
    inline float getHealth() { return health; }
    inline Vector2 *getPosition() { return &position; }
};