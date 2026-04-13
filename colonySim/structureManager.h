#pragma once
#include "structure.h"
#include "house.h"

enum class StructureType
{
    HouseType,
};

class StructureManager
{
private:
public:
    vector<Structure *> structureList;

    StructureManager();

    void createStructure(StructureType type, Tile *tile, Entity *owner);
};