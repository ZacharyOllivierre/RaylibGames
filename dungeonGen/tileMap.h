#pragma once

#include "tileData.h"
#include <vector>

using std::vector;

// Inherits from game object
// Inherits from on update
class TileMap
{
public:
    void resize(int width, int height);

    Tile &get(int x, int y);
    const Tile &get(int x, int y) const;
    void set(int x, int y, const Tile &value);

    int width() const;
    int height() const;

    vector<vector<Tile>> &data();
    const vector<vector<Tile>> &data() const;

    // OnUpdate()
    // {
    //     UpdateAnimations();
    // }

    // UpdateTile()
    // {

    // }

private:
    vector<vector<Tile>> tiles;
};

// update tile to

// struct Tile
// {
//     Vector2 position = {0, 0};
//     bool solid = false;
//     TileType type = TileType::Void;
//     int tileId = 0;

// bool hasAnimation
// Animation animation;
// };