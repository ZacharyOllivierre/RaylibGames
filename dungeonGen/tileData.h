#pragma once

#include <raylib.h>

struct MapConfig
{
    // Cant go lower than 6 for some reason
    Vector2 baseRoomSize = {6, 6};

    int numSubRooms = 7;
    Vector2 minSubRoomSize = {3, 3};

    // No lower than 2
    int minGap = 2;
};

enum class TileType
{
    Void,
    Floor,

    TopWall,
    BottomWall,
    LeftWall,
    RightWall,

    TopLeftCorner,
    TopRightCorner,
    BottomLeftCorner,
    BottomRightCorner,

    InnerTopLeft,
    InnerTopRight,
    InnerBottomLeft,
    InnerBottomRight,

    Count
};

struct Tile
{
    Vector2 position = {0, 0};
    bool solid = false;
    TileType type = TileType::Void;
    int tileId = 0;
};