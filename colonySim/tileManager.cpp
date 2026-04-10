#include "tileManager.h"

TileManager::TileManager(Vector2 tileAmount)
{
    this->tileAmount = tileAmount;

    initTileSet();
}

vector<Tile *> TileManager::getTiles()
{
    vector<Tile *> r;

    for (auto &tileRow : tileSet)
    {
        for (auto &tile : tileRow)
        {
            r.push_back(tile);
        }
    }
    return r;
}

void TileManager::initTileSet()
{
    tileSet.resize(tileAmount.y);

    for (int y = 0; y < tileAmount.y; y++)
    {
        for (int x = 0; x < tileAmount.x; x++)
        {
            Vector2 pos = {(float)x, (float)y};
            Tile *newTile = new Tile(GREEN, pos);

            tileSet[y].push_back(newTile);
        }
    }
}