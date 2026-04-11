#pragma once
#include <vector>
#include "entity.h"
#include "helperFunctions.h"

using std::vector;

class EntityManager
{
private:
    Rectangle simRec;

    void deleteListOfEntities(vector<int> indexes);

public:
    vector<Entity *> entityList;

    EntityManager(Rectangle simRec);

    void updateEntities();
    void createEntity(Vector2 pos, string name, float health, float speed);

    Entity *getRandomEntity();
    void killEntity(Entity *entity);
};