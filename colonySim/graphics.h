#pragma once
#include "raylib.h"
#include "tileManager.h"
#include "entityManager.h"

struct GraphicsData
{
    Rectangle screenRec;
    Rectangle simRec;
};

class Graphics
{
private:
    TileManager *tileManager;
    EntityManager *entityManager;

    GraphicsData *data;

    RenderTexture2D simTexture;
    RenderTexture2D entityTexture;

    Texture2D grassTexture;
    Texture2D baseEntityTexture;

    Vector2 tileSize;

    vector<Tile *> tileBuffer;

    void createSimTexture();
    void createEntityTexture();

    void printTile(Tile *tile);
    void printEntity(Entity *entity);

public:
    Graphics(GraphicsData *gd, TileManager *tm, EntityManager *em, Vector2 ts);
    ~Graphics();

    void printScreen();

    void addToTileBuffer(Tile *tile);
};