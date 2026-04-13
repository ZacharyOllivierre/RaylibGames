#include "structure.h"

Structure::Structure(Tile *tileLoc, Entity *owner, const char *texturePath)
{
    this->tileLoc = tileLoc;
    this->owner = owner;
    texture = LoadTexture(texturePath);
}

Structure::~Structure()
{
    UnloadTexture(texture);
}

void Structure::enterStructure(Entity *e)
{
    if (!e)
    {
        return;
    }

    entitiesInside.push_back(e);
}

void Structure::exitStructure(Entity *e)
{
    if (!e)
    {
        return;
    }
    // Erase entity from structure list
    entitiesInside.erase(std::remove(entitiesInside.begin(), entitiesInside.end(), e), entitiesInside.end());
}