#pragma once
#include "raylib.h"
#include <vector>

#include "tile.h"
using std::vector;

class TileManager
{
private:
    Vector2 tileAmount;

    void initTileSet();

public:
    vector<vector<Tile *>> tileSet;

    TileManager(Vector2 tileAmount);

    // Returns the tiles in 1d vector
    vector<Tile *> getTiles();
};