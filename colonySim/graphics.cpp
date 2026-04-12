#include "graphics.h"

Graphics::Graphics(GraphicsData *gd, TileManager *tm, EntityManager *em, Vector2 ts)
{
    tileManager = tm;
    entityManager = em;
    data = gd;
    tileSize = ts;

    // Start tile buffer with all tiles for init print
    tileBuffer = tileManager->getTiles();

    // Init textures
    simTexture = LoadRenderTexture(data->simRec.width, data->simRec.height);
    entityTexture = LoadRenderTexture(data->simRec.width, data->simRec.height);

    grassTexture = LoadTexture("grass.png");
    baseEntityTexture = LoadTexture("baseEntity.png");
}

Graphics::~Graphics()
{
    UnloadRenderTexture(simTexture);
    UnloadRenderTexture(entityTexture);

    UnloadTexture(grassTexture);
}

void Graphics::printScreen()
{
    // If tile buffer is not empty recreate sim texture
    if (!tileBuffer.empty())
    {
        createSimTexture();
    }

    // Create current entity texture
    createEntityTexture();

    BeginDrawing();

    ClearBackground(BLACK);

    DrawTexturePro(simTexture.texture,
                   {0, 0, (float)simTexture.texture.width, -(float)simTexture.texture.height},
                   data->simRec,
                   {0, 0}, 0, WHITE);

    DrawTexturePro(entityTexture.texture,
                   {0, 0, (float)entityTexture.texture.width, -(float)entityTexture.texture.height},
                   data->simRec,
                   {0, 0}, 0, WHITE);

    DrawFPS(10, 10);
    EndDrawing();
}

void Graphics::addToTileBuffer(Tile *tile)
{
    tileBuffer.push_back(tile);
}

// Private
void Graphics::createSimTexture()
{
    BeginTextureMode(simTexture);
    DrawRectangleRec(data->simRec, WHITE);

    // Draw each tile in buffer
    for (Tile *tile : tileBuffer)
    {
        printTile(tile);
    }

    tileBuffer.clear();

    EndTextureMode();
}

void Graphics::createEntityTexture()
{
    BeginTextureMode(entityTexture);

    ClearBackground(BLANK);

    for (Entity *entity : entityManager->entityList)
    {
        printEntity(entity);
    }
    EndTextureMode();
}

void Graphics::printTile(Tile *tile)
{
    Vector2 *tilePos = tile->getPosition();

    int xPos = tileSize.x * tilePos->x;
    int yPos = tileSize.y * tilePos->y;

    DrawTexturePro(grassTexture,
                   {0, 0, (float)grassTexture.width, (float)grassTexture.height},
                   {(float)xPos, (float)yPos, tileSize.x, tileSize.y},
                   {0, 0}, 0, WHITE);
}

void Graphics::printEntity(Entity *entity)
{
    Vector2 *entityPos = entity->getPosition();
    // Entity pixel size 8x8
    float entitySize = 8;

    DrawTexturePro(baseEntityTexture,
                   {0, 0, (float)baseEntityTexture.width, (float)baseEntityTexture.height},
                   {entityPos->x, entityPos->y, entitySize * 2, entitySize * 2},
                   {0, 0}, 0, WHITE);
}