#pragma once
#include "structure.h"

class House : public Structure
{
public:
    House(Tile *tileLoc, Entity *owner);

    void draw(Vector2 tileSize) const override;
};