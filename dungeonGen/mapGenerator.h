#pragma once

#include <raylib.h>

#include "tileData.h"
#include "tileMap.h"

enum class Side
{
    Top,
    Bottom,
    Left,
    Right,
    None
};

class MapGenerator
{
public:
    MapGenerator(const Vector2 gridDimensions, MapConfig config, TileMap &grid);

    void generateRoom();
    void clearGrid();

    inline TileMap &getGrid() { return grid; }
    inline const TileMap &getGrid() const { return grid; }

private:
    void initGrid();

    Rectangle buildBaseRoom();
    Rectangle buildSubRoom(Rectangle &base, Side side = Side::None);

    void addRecToGrid(Rectangle &rec);
    void classifyRecs();

    bool isSolid(const int x, const int y);

    void assignTileIds();
    void resetTile(Tile &tile);

private:
    Vector2 _gridDimensions;
    TileMap &grid;

    MapConfig config;
};