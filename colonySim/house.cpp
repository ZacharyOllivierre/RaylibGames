#include "house.h"

House::House(Tile *tileLoc, Entity *owner, const char *texturePath) : Structure(tileLoc, owner, texturePath)
{
}

void House::draw() const
{
    if (!tileLoc)
    {
        return;
    }

    DrawTexturePro(texture,
                   {0, 0, (float)texture.width, (float)texture.height},
                   {tileLoc->getPosition()->x, tileLoc->getPosition()->y, (float)texture.width, (float)texture.height},
                   {0, 0}, 0, WHITE);
}