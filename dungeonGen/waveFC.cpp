#include "waveFC.h"

#include <algorithm>
#include <limits>
#include <queue>
#include <utility>

#include "tileSet.h"

namespace
{
    constexpr int TILE_SIZE = 32;
    constexpr const char *TILESET_IMAGE_PATH = "assets/NewGameTileSheet.png";

    bool tilesMatch(const Color *samplePixels,
                    int sampleWidth,
                    int sampleStartX,
                    int sampleStartY,
                    const Color *atlasPixels,
                    int atlasWidth,
                    int atlasStartX,
                    int atlasStartY)
    {
        for (int y = 0; y < TILE_SIZE; y++)
        {
            for (int x = 0; x < TILE_SIZE; x++)
            {
                const Color sampleColor = samplePixels[(sampleStartY + y) * sampleWidth + (sampleStartX + x)];
                const Color atlasColor = atlasPixels[(atlasStartY + y) * atlasWidth + (atlasStartX + x)];

                if (sampleColor.r != atlasColor.r ||
                    sampleColor.g != atlasColor.g ||
                    sampleColor.b != atlasColor.b ||
                    sampleColor.a != atlasColor.a)
                {
                    return false;
                }
            }
        }

        return true;
    }
}

WaveFC::WaveFC(TileMap &grid)
{
    this->grid = &grid;
    initialize();
}

bool WaveFC::loadSampleImage(const std::string &path)
{
    Image image = LoadImage(path.c_str());
    Image atlas = LoadImage(TILESET_IMAGE_PATH);

    if (image.data == nullptr || image.width <= 0 || image.height <= 0 || atlas.data == nullptr || atlas.width <= 0 || atlas.height <= 0)
    {
        if (image.data != nullptr)
        {
            UnloadImage(image);
        }
        if (atlas.data != nullptr)
        {
            UnloadImage(atlas);
        }
        return false;
    }

    if (image.width % TILE_SIZE != 0 || image.height % TILE_SIZE != 0 || atlas.width % TILE_SIZE != 0 || atlas.height % TILE_SIZE != 0)
    {
        UnloadImage(image);
        UnloadImage(atlas);
        return false;
    }

    Color *pixels = LoadImageColors(image);
    Color *atlasPixels = LoadImageColors(atlas);

    if (pixels == nullptr || atlasPixels == nullptr)
    {
        if (pixels != nullptr)
        {
            UnloadImageColors(pixels);
        }
        if (atlasPixels != nullptr)
        {
            UnloadImageColors(atlasPixels);
        }
        UnloadImage(image);
        UnloadImage(atlas);
        return false;
    }

    const int sampleCols = image.width / TILE_SIZE;
    const int sampleRows = image.height / TILE_SIZE;

    const int atlasCols = atlas.width / TILE_SIZE;
    const int atlasRows = atlas.height / TILE_SIZE;
    const int atlasTileCount = atlasCols * atlasRows;

    sampleGrid.resize(sampleCols, sampleRows);

    std::unordered_map<int, int> sampleMatches;
    int unmatchedTiles = 0;

    for (int y = 0; y < sampleRows; y++)
    {
        for (int x = 0; x < sampleCols; x++)
        {
            const int sampleStartX = x * TILE_SIZE;
            const int sampleStartY = y * TILE_SIZE;

            int matchedTileId = -1;

            for (int tileId = 0; tileId < atlasTileCount; tileId++)
            {
                const int atlasX = (tileId % atlasCols) * TILE_SIZE;
                const int atlasY = (tileId / atlasCols) * TILE_SIZE;

                if (tilesMatch(pixels,
                               image.width,
                               sampleStartX,
                               sampleStartY,
                               atlasPixels,
                               atlas.width,
                               atlasX,
                               atlasY))
                {
                    matchedTileId = tileId;
                    break;
                }
            }

            Tile &tile = sampleGrid.data()[y][x];
            tile.position = {(float)x, (float)y};

            if (matchedTileId >= 0)
            {
                tile.type = TileType::Floor;
                tile.solid = true;
                tile.tileId = matchedTileId;
                if (TileSet::isWfcFloorTileId(matchedTileId))
                {
                    sampleMatches[matchedTileId]++;
                }
                continue;
            }

            unmatchedTiles++;
            tile.type = TileType::Void;
            tile.solid = false;
            tile.tileId = TileSet::idForType(TileType::Void);
        }
    }

    UnloadImageColors(pixels);
    UnloadImageColors(atlasPixels);
    UnloadImage(image);
    UnloadImage(atlas);

    useSampleImage = true;
    initialize();

    const int firstFloorTileId = TileSet::firstWfcFloorTileId();
    const int lastFloorTileId = atlasTileCount - 1;
    int matchedFloorKinds = 0;
    vector<int> missingTiles;

    for (int tileId = firstFloorTileId; tileId <= lastFloorTileId; tileId++)
    {
        if (sampleMatches.find(tileId) != sampleMatches.end())
        {
            matchedFloorKinds++;
        }
        else
        {
            missingTiles.push_back(tileId);
        }
    }

    TraceLog(LOG_INFO, "WFC sample loaded: %d/%d floor tile kinds matched, %d sample tiles unmatched", matchedFloorKinds, std::max(0, lastFloorTileId - firstFloorTileId + 1), unmatchedTiles);

    if (!missingTiles.empty())
    {
        std::string missingList = "WFC sample missing tile ids:";
        for (int tileId : missingTiles)
        {
            missingList += " ";
            missingList += std::to_string(tileId);
        }
        TraceLog(LOG_INFO, "%s", missingList.c_str());
    }

    return true;
}

void WaveFC::clearSampleImage()
{
    useSampleImage = false;
    sampleGrid = TileMap{};
    initialize();
}

void WaveFC::rebuild()
{
    initialize();
}

bool WaveFC::step()
{
    if (finished())
    {
        return false;
    }

    int x = 0;
    int y = 0;

    if (!findLowestEntropy(x, y))
    {
        return false;
    }

    collapse(x, y);
    return propagate(x, y);
}

void WaveFC::solve()
{
    while (!finished())
    {
        if (!step())
        {
            break;
        }
    }
}

bool WaveFC::finished() const
{
    if (hasContradiction || wave.empty())
    {
        return true;
    }

    for (const auto &row : wave)
    {
        for (const WaveCell &cell : row)
        {
            if (!cell.collapsed)
            {
                return false;
            }
        }
    }

    return true;
}

const TileMap &WaveFC::getOutput() const
{
    return output;
}

const vector<vector<WaveCell>> &WaveFC::getWave() const
{
    return wave;
}

void WaveFC::initialize()
{
    hasContradiction = false;
    floorWeights.clear();
    adjacency.clear();
    floorDomain.clear();
    wave.clear();
    mutableFloor.clear();
    output = TileMap{};

    if (grid == nullptr)
    {
        return;
    }

    if (grid->width() <= 0 || grid->height() <= 0)
    {
        return;
    }

    patternSource = useSampleImage ? &sampleGrid : grid;

    if (patternSource == nullptr || patternSource->width() <= 0 || patternSource->height() <= 0)
    {
        patternSource = grid;
    }

    output.resize(grid->width(), grid->height());
    output.data() = grid->data();

    wave.resize(grid->height());
    mutableFloor.resize(grid->height());

    extractPatterns();

    if (floorDomain.empty())
    {
        return;
    }

    for (int y = 0; y < grid->height(); y++)
    {
        wave[y].resize(grid->width());
        mutableFloor[y].resize(grid->width());

        for (int x = 0; x < grid->width(); x++)
        {
            const Tile &source = grid->data()[y][x];

            if (TileSet::isFloorType(source.type))
            {
                wave[y][x].collapsed = false;
                wave[y][x].possibilities = floorDomain;
                mutableFloor[y][x] = true;
            }
            else
            {
                wave[y][x].collapsed = true;
                wave[y][x].possibilities = {source.tileId};
                mutableFloor[y][x] = false;
            }

            syncOutputCell(x, y);
        }
    }

    for (int y = 0; y < grid->height(); y++)
    {
        for (int x = 0; x < grid->width(); x++)
        {
            if (!wave[y][x].collapsed)
            {
                if (!propagate(x, y))
                {
                    return;
                }
            }
        }
    }
}

void WaveFC::extractPatterns()
{
    const auto &tiles = patternSource->data();
    const int voidTileId = TileSet::idForType(TileType::Void);

    for (int y = 0; y < patternSource->height(); y++)
    {
        for (int x = 0; x < patternSource->width(); x++)
        {
            if (tiles[y][x].tileId != voidTileId)
            {
                int tileValue = tiles[y][x].tileId;
                if (TileSet::isWfcFloorTileId(tileValue))
                {
                    floorWeights[tileValue]++;

                    if (floorWeights[tileValue] == 1)
                    {
                        floorDomain.push_back(tileValue);
                    }
                }
            }
        }
    }

    if (patternSource->width() < overlapSize || patternSource->height() < overlapSize)
    {
        return;
    }

    for (int y = 0; y <= patternSource->height() - overlapSize; y++)
    {
        for (int x = 0; x <= patternSource->width() - overlapSize; x++)
        {
            for (int row = 0; row < overlapSize; row++)
            {
                for (int col = 0; col < overlapSize - 1; col++)
                {
                    const Tile &left = tiles[y + row][x + col];
                    const Tile &right = tiles[y + row][x + col + 1];

                    if (left.tileId != voidTileId && right.tileId != voidTileId)
                    {
                        adjacency[left.tileId][directionIndex(1, 0)].insert(right.tileId);
                        adjacency[right.tileId][directionIndex(-1, 0)].insert(left.tileId);
                    }
                }
            }

            for (int col = 0; col < overlapSize; col++)
            {
                for (int row = 0; row < overlapSize - 1; row++)
                {
                    const Tile &top = tiles[y + row][x + col];
                    const Tile &bottom = tiles[y + row + 1][x + col];

                    if (top.tileId != voidTileId && bottom.tileId != voidTileId)
                    {
                        adjacency[top.tileId][directionIndex(0, 1)].insert(bottom.tileId);
                        adjacency[bottom.tileId][directionIndex(0, -1)].insert(top.tileId);
                    }
                }
            }
        }
    }
}

bool WaveFC::findLowestEntropy(int &x, int &y) const
{
    int lowestEntropy = std::numeric_limits<int>::max();
    vector<std::pair<int, int>> candidates;

    for (int row = 0; row < (int)wave.size(); row++)
    {
        for (int col = 0; col < (int)wave[row].size(); col++)
        {
            const WaveCell &cell = wave[row][col];

            if (!isMutableFloor(col, row))
            {
                continue;
            }

            if (cell.collapsed || cell.possibilities.size() <= 1)
            {
                continue;
            }

            int entropy = (int)cell.possibilities.size();
            if (entropy < lowestEntropy)
            {
                lowestEntropy = entropy;
                candidates.clear();
                candidates.push_back({col, row});
            }
            else if (entropy == lowestEntropy)
            {
                candidates.push_back({col, row});
            }
        }
    }

    if (candidates.empty())
    {
        return false;
    }

    const int choice = GetRandomValue(0, (int)candidates.size() - 1);
    x = candidates[choice].first;
    y = candidates[choice].second;
    return true;
}

void WaveFC::collapse(int x, int y)
{
    WaveCell &cell = wave[y][x];

    if (cell.possibilities.empty())
    {
        hasContradiction = true;
        return;
    }

    int totalWeight = 0;
    for (int tileValue : cell.possibilities)
    {
        totalWeight += floorWeights[tileValue];
    }

    int roll = GetRandomValue(1, totalWeight);
    int chosenValue = cell.possibilities.front();

    for (int tileValue : cell.possibilities)
    {
        roll -= floorWeights[tileValue];
        if (roll <= 0)
        {
            chosenValue = tileValue;
            break;
        }
    }

    cell.possibilities = {chosenValue};
    cell.collapsed = true;
    syncOutputCell(x, y);
}

bool WaveFC::propagate(int startX, int startY)
{
    std::queue<std::pair<int, int>> pending;
    pending.push({startX, startY});

    while (!pending.empty())
    {
        const auto [x, y] = pending.front();
        pending.pop();

        static const std::pair<int, int> OFFSETS[] = {
            {0, -1},
            {0, 1},
            {-1, 0},
            {1, 0}};

        for (const auto &[dx, dy] : OFFSETS)
        {
            int neighborX = x + dx;
            int neighborY = y + dy;

            if (neighborY < 0 || neighborY >= (int)wave.size() || neighborX < 0 || neighborX >= (int)wave[neighborY].size())
            {
                continue;
            }

            if (filterNeighbor(neighborX, neighborY, x, y, dx, dy))
            {
                pending.push({neighborX, neighborY});
            }

            if (hasContradiction)
            {
                return false;
            }
        }
    }

    return true;
}

bool WaveFC::filterNeighbor(int x, int y, int fromX, int fromY, int dx, int dy)
{
    WaveCell &neighbor = wave[y][x];
    const WaveCell &source = wave[fromY][fromX];

    if (!isMutableFloor(x, y))
    {
        return false;
    }

    if (!isMutableFloor(fromX, fromY))
    {
        return false;
    }

    vector<int> filtered;
    filtered.reserve(neighbor.possibilities.size());

    for (int neighborValue : neighbor.possibilities)
    {
        bool compatible = false;

        for (int sourceValue : source.possibilities)
        {
            if (valuesCompatible(sourceValue, neighborValue, dx, dy))
            {
                compatible = true;
                break;
            }
        }

        if (compatible)
        {
            filtered.push_back(neighborValue);
        }
    }

    if (filtered.empty())
    {
        hasContradiction = true;
        return false;
    }

    if (filtered.size() == neighbor.possibilities.size())
    {
        return false;
    }

    neighbor.possibilities = filtered;
    neighbor.collapsed = neighbor.possibilities.size() == 1;

    if (neighbor.collapsed)
    {
        syncOutputCell(x, y);
    }

    return true;
}

bool WaveFC::valuesCompatible(int sourceValue, int neighborValue, int dx, int dy) const
{
    int direction = directionIndex(dx, dy);

    if (direction < 0)
    {
        return false;
    }

    auto sourceIt = adjacency.find(sourceValue);

    // If we do not have overlap samples for this value, keep it permissive.
    if (sourceIt == adjacency.end())
    {
        return true;
    }

    const auto &allowed = sourceIt->second[direction];

    if (allowed.empty())
    {
        return true;
    }

    return allowed.find(neighborValue) != allowed.end();
}

int WaveFC::directionIndex(int dx, int dy) const
{
    if (dx == 0 && dy == -1)
    {
        return 0;
    }

    if (dx == 0 && dy == 1)
    {
        return 1;
    }

    if (dx == -1 && dy == 0)
    {
        return 2;
    }

    if (dx == 1 && dy == 0)
    {
        return 3;
    }

    return -1;
}

bool WaveFC::isMutableFloor(int x, int y) const
{
    if (y < 0 || y >= (int)mutableFloor.size())
    {
        return false;
    }

    if (x < 0 || x >= (int)mutableFloor[y].size())
    {
        return false;
    }

    return mutableFloor[y][x];
}

void WaveFC::syncOutputCell(int x, int y)
{
    if (y < 0 || y >= (int)wave.size())
    {
        return;
    }

    if (x < 0 || x >= (int)wave[y].size())
    {
        return;
    }

    if (!wave[y][x].collapsed || wave[y][x].possibilities.empty())
    {
        return;
    }

    int tileValue = wave[y][x].possibilities.front();
    Tile &tile = output.data()[y][x];
    tile.position = {(float)x, (float)y};
    tile.tileId = tileValue;

    if (isMutableFloor(x, y))
    {
        tile.type = TileType::Floor;
        tile.solid = true;
    }
}