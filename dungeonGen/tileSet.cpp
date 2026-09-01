#include "tileSet.h"

namespace
{
    constexpr std::array<int, static_cast<int>(TileType::Count)> TILE_IDS = {
        13, // Void
        12, // Floor
        0,  // TopWall
        1,  // BottomWall
        2,  // LeftWall
        3,  // RightWall
        5,  // TopLeftCorner
        4,  // TopRightCorner
        6,  // BottomLeftCorner
        7,  // BottomRightCorner
        10, // InnerTopLeft
        11, // InnerTopRight
        8,  // InnerBottomLeft
        9   // InnerBottomRight
    };
}

int TileSet::idForType(TileType type)
{
    return TILE_IDS[(int)type];
}

bool TileSet::isFloorType(TileType type)
{
    return type == TileType::Floor;
}

int TileSet::firstWfcFloorTileId()
{
    return idForType(TileType::Void) + 1;
}

bool TileSet::isWfcFloorTileId(int tileId)
{
    return tileId >= firstWfcFloorTileId();
}

bool TileSet::load(const string &path, int tileSize)
{
    this->tileSize = tileSize;
    tiles.clear();
    texture = LoadTexture(path.c_str());

    if (texture.id == 0 || tileSize <= 0)
    {
        return false;
    }

    int cols = texture.width / tileSize;
    int rows = texture.height / tileSize;

    tiles.reserve(cols * rows);

    int id = 0;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            tiles.push_back({id++, {(float)x * tileSize, (float)y * tileSize, (float)tileSize, (float)tileSize}});
        }
    }
    return true;
}

const TileDefinition &TileSet::getTile(int id) const
{
    return tiles[id];
}

const Texture2D &TileSet::getTexture() const
{
    return texture;
}

int TileSet::getTileSize() const
{
    return tileSize;
}

int TileSet::getTileCount() const
{
    return (int)tiles.size();
}