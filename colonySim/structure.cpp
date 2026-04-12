#include "structure.h"

Structure::Structure(Tile *tileLoc, const char *texturePath)
{
    this->tileLoc = tileLoc;
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

    e->insideStructure = true;
}

void Structure::exitStructure(Entity *e)
{
    if (!e)
    {
        return;
    }
    // Erase entity from structure list
    entitiesInside.erase(std::remove(entitiesInside.begin(), entitiesInside.end(), e), entitiesInside.end());

    e->insideStructure = false;
}