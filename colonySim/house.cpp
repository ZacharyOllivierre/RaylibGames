#include "house.h"

House::House(Tile *tileLoc, Entity *owner) : Structure(tileLoc, owner, "resources/house.png")
{
}

void House::draw(Vector2 tileSize) const
{
    if (!tileLoc)
    {
        return;
    }

    Vector2 *tilePos = tileLoc->getPosition();
    float xPos = tilePos->x * tileSize.x;
    float yPos = tilePos->y * tileSize.y;

    DrawTexturePro(texture,
                   {0, 0, (float)texture.width, (float)texture.height},
                   {xPos, yPos, tileSize.x, tileSize.y},
                   {0, 0}, 0, WHITE);
}