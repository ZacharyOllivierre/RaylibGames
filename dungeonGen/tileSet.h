#pragma once

#include <raylib.h>
#include <array>
#include <vector>
#include <string>

#include "tileData.h"

using std::string;

struct TileDefinition
{
    int id;
    Rectangle source;
};

class TileSet
{
public:
    static int idForType(TileType type);
    static bool isFloorType(TileType type);
    static int firstWfcFloorTileId();
    static bool isWfcFloorTileId(int tileId);

    bool load(const string &path, int tileSize);

    const TileDefinition &getTile(int id) const;
    const Texture2D &getTexture() const;

    int getTileSize() const;
    int getTileCount() const;

private:
    Texture2D texture{};
    std::vector<TileDefinition> tiles;

    int tileSize;
};