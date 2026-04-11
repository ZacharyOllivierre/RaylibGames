#include "entityManager.h"

EntityManager::EntityManager(Rectangle simRec)
{
    this->simRec = simRec;
}

void EntityManager::updateEntities()
{
    // Stores indexes of dead entities and deletes them after updating
    vector<int> indexesToDelete;

    // Access each entity in list
    int size = entityList.size();
    for (int i = 0; i < size; i++)
    {
        Entity *entity = entityList[i];

        // Update all entities
        entity->update();

        // Add to indexes to delete if dead
        if (!entity->getAlive())
        {
            indexesToDelete.push_back(i);
        }
    }

    // Delete dead entites
    deleteListOfEntities(indexesToDelete);
}

void EntityManager::createEntity()
{
    Vector2 pos = getCenterOfRec(&simRec);
    Entity *newEntity = new Entity(pos, "Test Entity", 100, 3, true);

    entityList.push_back(newEntity);
}

// private
void EntityManager::deleteListOfEntities(vector<int> indexes)
{
    // Deletes from end to avoid changing indx of unprocessed entities
    for (int i = indexes.size() - 1; i >= 0; i--)
    {
        delete entityList[indexes[i]];
        entityList.erase(entityList.begin() + indexes[i]);
    }
}