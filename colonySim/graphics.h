#pragma once
#include "raylib.h"
#include "tileManager.h"
#include "entityManager.h"
#include "structureManager.h"

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
    StructureManager *structManager;

    Camera2D *camera;

    GraphicsData *data;

    RenderTexture2D simTexture;
    RenderTexture2D entityTexture;
    RenderTexture2D structTexture;

    Texture2D grassTexture;
    Texture2D baseEntityTexture;

    Vector2 tileSize;

    vector<Tile *> tileBuffer;

    void createSimTexture();
    void createEntityTexture();
    void createStructureTexture();

    void printTile(Tile *tile);
    void printEntity(Entity *entity);

public:
    Graphics(GraphicsData *gd, TileManager *tm, EntityManager *em, StructureManager *sm, Vector2 ts, Camera2D *c);
    ~Graphics();

    void printScreen();

    void addToTileBuffer(Tile *tile);
};