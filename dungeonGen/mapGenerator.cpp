#include "mapGenerator.h"

#include "tileSet.h"

MapGenerator::MapGenerator(const Vector2 gridDimensions, MapConfig config, TileMap &grid)
    : _gridDimensions(gridDimensions), grid(grid), config(config)
{
    initGrid();
}

void MapGenerator::generateRoom()
{
    clearGrid();

    Rectangle base = buildBaseRoom();
    addRecToGrid(base);

    for (int i = 0; i < config.numSubRooms; i++)
    {
        Rectangle subRoom = buildSubRoom(base);
        addRecToGrid(subRoom);
    }

    classifyRecs();
    assignTileIds();
}

void MapGenerator::clearGrid()
{
    auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            resetTile(tiles[y][x]);
        }
    }
}

void MapGenerator::initGrid()
{
    grid.resize((int)_gridDimensions.x, (int)_gridDimensions.y);

    auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            tiles[y][x].position = {(float)x, (float)y};
            resetTile(tiles[y][x]);
        }
    }
}

Rectangle MapGenerator::buildBaseRoom()
{
    Rectangle room = {0, 0, config.baseRoomSize.x, config.baseRoomSize.y};
    Vector2 center = {_gridDimensions.x / 2, _gridDimensions.y / 2};

    room.x = center.x - (room.width / 2);
    room.y = center.y - (room.height / 2);

    return room;
}

// Builds room adjacent to main room, bounded by grid size / minimum gap
Rectangle MapGenerator::buildSubRoom(Rectangle &base, Side side)
{
    // Choose side for generation of sub room (if not given)
    if (side == Side::None)
    {
        side = (Side)GetRandomValue((int)Side::Top, (int)Side::Right);
    }

    Rectangle room;

    int baseRight = base.x + base.width - 1;
    int baseBottom = base.y + base.height - 1;

    Vector2 minSize = config.minSubRoomSize;
    Vector2 anchorPoint;

    /*When taking only outer most edges of full shape minGap space
    will always be walkable between rooms*/
    int minGap = config.minGap;

    // Whether square is building forward or back from anchor point
    bool generateForward = GetRandomValue(0, 1);

    // Top / Bottom Generation
    if (side == Side::Top || side == Side::Bottom)
    {
        // Top anchor and height
        if (side == Side::Top)
        {
            // Choose anchor point, connection to base room
            anchorPoint.y = base.y;
            anchorPoint.x = GetRandomValue(base.x + (minGap + 1), baseRight - (minGap + 1));

            room.height = GetRandomValue(minSize.y, anchorPoint.y);
            room.y = anchorPoint.y - room.height + 1;
        }
        // Bottom anchor and height
        else
        {
            anchorPoint.y = baseBottom;
            anchorPoint.x = GetRandomValue(base.x + (minGap + 1), baseRight - (minGap + 1));

            room.height = GetRandomValue(minSize.y, _gridDimensions.y - anchorPoint.y);
            room.y = anchorPoint.y;
        }

        // Shared forward or backward width and x
        if (generateForward)
        {
            room.width = GetRandomValue(minSize.x, _gridDimensions.x - anchorPoint.x);
            room.x = anchorPoint.x;
        }
        else
        {
            room.width = GetRandomValue(minSize.x, anchorPoint.x);
            room.x = anchorPoint.x - room.width + 1;
        }
    }
    // Left / Right generation
    else
    {
        // Left anchor point and width
        if (side == Side::Left)
        {
            anchorPoint.x = base.x;
            anchorPoint.y = GetRandomValue(base.y + (minGap + 1), baseBottom - (minGap + 1));

            room.width = GetRandomValue(minSize.x, anchorPoint.x);
            room.x = anchorPoint.x - room.width + 1;
        }
        // Right anchor point and width
        else
        {
            anchorPoint.x = baseRight;
            anchorPoint.y = GetRandomValue(base.y + (minGap + 1), baseBottom - (minGap + 1));

            room.width = GetRandomValue(minSize.x, _gridDimensions.x - anchorPoint.x);
            room.x = anchorPoint.x;
        }

        // Shared forward back height and y point
        if (generateForward)
        {
            room.height = GetRandomValue(minSize.y, _gridDimensions.y - anchorPoint.y);
            room.y = anchorPoint.y;
        }
        else
        {
            room.height = GetRandomValue(minSize.y, anchorPoint.y);
            room.y = anchorPoint.y - room.height + 1;
        }
    }

    return room;
}

// Updates grid with rec, sets walls and inside to standard type
void MapGenerator::addRecToGrid(Rectangle &rec)
{
    auto &tiles = grid.data();

    for (int y = rec.y; y < rec.y + rec.height; y++)
    {
        for (int x = rec.x; x < rec.x + rec.width; x++)
        {
            tiles[y][x].solid = true;
            tiles[y][x].type = TileType::Floor;
        }
    }
}

void MapGenerator::classifyRecs()
{
    auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            Tile *tile = &tiles[y][x];

            if (tile->solid == false)
            {
                continue;
            }

            bool N = isSolid(x, y - 1);
            bool S = isSolid(x, y + 1);
            bool E = isSolid(x + 1, y);
            bool W = isSolid(x - 1, y);

            bool NE = isSolid(x + 1, y - 1);
            bool NW = isSolid(x - 1, y - 1);
            bool SE = isSolid(x + 1, y + 1);
            bool SW = isSolid(x - 1, y + 1);

            // Outer corners
            if (!N && !W)
                tile->type = TileType::TopLeftCorner;

            else if (!N && !E)
                tile->type = TileType::TopRightCorner;

            else if (!S && !W)
                tile->type = TileType::BottomLeftCorner;

            else if (!S && !E)
                tile->type = TileType::BottomRightCorner;

            // Inner corners
            else if (N && W && !NW)
                tile->type = TileType::InnerTopLeft;

            else if (N && E && !NE)
                tile->type = TileType::InnerTopRight;

            else if (S && W && !SW)
                tile->type = TileType::InnerBottomLeft;

            else if (S && E && !SE)
                tile->type = TileType::InnerBottomRight;

            // Walls
            else if (!N)
                tile->type = TileType::TopWall;

            else if (!S)
                tile->type = TileType::BottomWall;

            else if (!W)
                tile->type = TileType::LeftWall;

            else if (!E)
                tile->type = TileType::RightWall;

            // Interior
            else
                tile->type = TileType::Floor;
        }
    }
}

bool MapGenerator::isSolid(const int x, const int y)
{
    if (x < 0 || x >= _gridDimensions.x)
        return false;

    if (y < 0 || y >= _gridDimensions.y)
        return false;

    return grid.data()[y][x].solid;
}

// Final Pass to update grid with tileIds
void MapGenerator::assignTileIds()
{
    auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            Tile &tile = tiles[y][x];
            tile.tileId = TileSet::idForType(tile.type);
        }
    }
}

void MapGenerator::resetTile(Tile &tile)
{
    tile.solid = false;
    tile.type = TileType::Void;
    tile.tileId = TileSet::idForType(TileType::Void);
}