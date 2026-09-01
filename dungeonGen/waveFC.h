#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "tileMap.h"

using std::vector;

struct WaveCell
{
    bool collapsed = false;
    vector<int> possibilities;
};

class WaveFC
{
public:
    explicit WaveFC(TileMap &grid);

    bool loadSampleImage(const std::string &path);
    void clearSampleImage();

    void rebuild();
    bool step();
    void solve();

    bool finished() const;

    const TileMap &getOutput() const;
    const vector<vector<WaveCell>> &getWave() const;

private:
    void initialize();
    void extractPatterns();

    bool findLowestEntropy(int &x, int &y) const;
    void collapse(int x, int y);
    bool propagate(int startX, int startY);
    bool filterNeighbor(int x, int y, int fromX, int fromY, int dx, int dy);
    bool valuesCompatible(int sourceValue, int neighborValue, int dx, int dy) const;
    int directionIndex(int dx, int dy) const;
    bool isMutableFloor(int x, int y) const;
    void syncOutputCell(int x, int y);

private:
    TileMap *grid = nullptr;
    TileMap sampleGrid;
    const TileMap *patternSource = nullptr;
    TileMap output;
    vector<vector<WaveCell>> wave;
    vector<vector<bool>> mutableFloor;

    std::unordered_map<int, int> floorWeights;
    std::unordered_map<int, std::array<std::unordered_set<int>, 4>> adjacency;
    vector<int> floorDomain;

    int overlapSize = 3;
    bool useSampleImage = false;
    bool hasContradiction = false;
};