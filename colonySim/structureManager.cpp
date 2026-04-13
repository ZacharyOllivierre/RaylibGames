#include "structureManager.h"

StructureManager::StructureManager()
{
}

void StructureManager::createStructure(StructureType type, Tile *tile, Entity *owner)
{
    Structure *newStructure = nullptr;

    switch (type)
    {
    case StructureType::HouseType:
        newStructure = new House(tile, owner);
        break;
    }

    structureList.push_back(newStructure);
}