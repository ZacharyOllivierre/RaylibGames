#pragma once
#include "structure.h"

class House : protected Structure
{
public:
    House(Tile *tileLoc, const char *texturePath);

    void draw() const override;
};