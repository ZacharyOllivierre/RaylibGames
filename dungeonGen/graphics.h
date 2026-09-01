#pragma once
#include "raylib.h"

#include "mapGenerator.h"
#include "waveFC.h"
#include "tileSet.h"

enum class GraphicsView
{
    GeneratedMap = 0,
    WaveResult = 1,
    WaveEntropy = 2
};

class Graphics
{
public:
    Graphics(MapGenerator *map, WaveFC *wave, Vector2 screenDimensions, Vector2 gridDimensions);

    void draw();
    void toggleView();

private:
    MapGenerator *map;
    WaveFC *wave;

    GraphicsView view = GraphicsView::GeneratedMap;

    Vector2 _screenDimensions;
    Vector2 _gridDimensions;
    Vector2 _tileSize;

    TileSet tileSet;

private:
    void drawGrid(Vector2 dimensions);
    void drawGeneratedMap();
    void drawWaveResult();
    void drawWaveEntropy();
    void drawTileMap(const TileMap &grid, Vector2 dimensions);
};