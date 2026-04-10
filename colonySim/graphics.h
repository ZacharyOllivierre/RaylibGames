#pragma once
#include "raylib.h"
#include "tileManager.h"

struct GraphicsData
{
    Rectangle screenRec;
    Rectangle simRec;
};

class Graphics
{
private:
    TileManager *tileManager;

    GraphicsData *data;

    RenderTexture2D simTexture;
    Texture2D grassTexture;

    Vector2 tileSize;

    vector<Tile *> tileBuffer;

    void createSimTexture();
    void printTile(Tile *tile);

public:
    Graphics(GraphicsData *gd, TileManager *tm, Vector2 ts);
    ~Graphics();

    void printScreen();

    void addToTileBuffer(Tile *tile);
};