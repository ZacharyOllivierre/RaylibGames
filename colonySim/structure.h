#pragma once
#include "tile.h"
#include "entity.h"
#include <vector>

using std::vector;

class Structure
{
protected:
    Tile *tileLoc;
    Entity *owner;

    Texture2D texture;

    vector<Entity *> entitiesInside;

public:
    Structure(Tile *tileLoc, Entity *owner, const char *texturePath);
    virtual ~Structure();

    virtual void draw() const = 0;

    void enterStructure(Entity *e);
    void exitStructure(Entity *e);

    inline Tile *getTile() { return tileLoc; }
};