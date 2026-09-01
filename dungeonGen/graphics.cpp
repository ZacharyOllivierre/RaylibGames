#include "graphics.h"

Graphics::Graphics(MapGenerator *map, WaveFC *wave, Vector2 screenDimensions, Vector2 gridDimensions)
{
    this->map = map;
    this->wave = wave;
    _screenDimensions = screenDimensions;
    _gridDimensions = gridDimensions;

    _tileSize.x = _screenDimensions.x / _gridDimensions.x;
    _tileSize.y = _screenDimensions.y / _gridDimensions.y;

    int spriteSize = 32;
    tileSet.load("assets/NewGameTileSheet.png", spriteSize);
}

void Graphics::draw()
{
    BeginDrawing();
    ClearBackground(WHITE);

    switch (view)
    {
    case GraphicsView::GeneratedMap:
        drawGeneratedMap();
        break;
    case GraphicsView::WaveResult:
        drawWaveResult();
        break;
    case GraphicsView::WaveEntropy:
        drawWaveEntropy();
        break;
    }

    EndDrawing();
}

void Graphics::toggleView()
{
    view = static_cast<GraphicsView>(((int)view + 1) % 3);
}

void Graphics::drawGrid(Vector2 dimensions)
{
    if (dimensions.x <= 0 || dimensions.y <= 0)
    {
        return;
    }

    int hSpacing = _screenDimensions.x / dimensions.x;
    int vSpacing = _screenDimensions.y / dimensions.y;

    // Vert lines
    for (int x = hSpacing; x < _screenDimensions.x; x += hSpacing)
    {
        DrawLine(x, 0, x, _screenDimensions.y, RED);
    }

    // Hor lines
    for (int y = vSpacing; y < _screenDimensions.y; y += vSpacing)
    {
        DrawLine(0, y, _screenDimensions.x, y, RED);
    }
}

void Graphics::drawGeneratedMap()
{
    drawTileMap(map->getGrid(), _gridDimensions);
    // drawGrid(_gridDimensions);
}

void Graphics::drawWaveResult()
{
    if (wave == nullptr)
    {
        return;
    }

    const TileMap &output = wave->getOutput();
    Vector2 dimensions = {(float)output.width(), (float)output.height()};

    if (dimensions.x <= 0 || dimensions.y <= 0)
    {
        return;
    }

    drawTileMap(output, dimensions);
    // drawGrid(dimensions);
}

void Graphics::drawWaveEntropy()
{
    if (wave == nullptr)
    {
        return;
    }

    const auto &cells = wave->getWave();
    const TileMap &output = wave->getOutput();

    if (cells.empty())
    {
        return;
    }

    Vector2 dimensions = {(float)cells[0].size(), (float)cells.size()};
    Vector2 tileSize = {_screenDimensions.x / dimensions.x, _screenDimensions.y / dimensions.y};

    drawTileMap(output, dimensions);

    for (int y = 0; y < (int)cells.size(); y++)
    {
        for (int x = 0; x < (int)cells[y].size(); x++)
        {
            if (output.data()[y][x].type != TileType::Floor)
            {
                continue;
            }

            const WaveCell &cell = cells[y][x];
            int possibilities = (int)cell.possibilities.size();
            float ratio = possibilities > 0 ? 1.0f / (float)possibilities : 1.0f;
            Color fill = Color{(unsigned char)(50 + 180 * ratio), (unsigned char)(70 + 90 * ratio), 200, 160};

            Rectangle rect = {x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y};
            DrawRectangleRec(rect, fill);
            DrawRectangleLinesEx(rect, 1, ColorAlpha(BLACK, 0.25f));
            DrawText(TextFormat("%d", possibilities), (int)(rect.x + 4), (int)(rect.y + 4), 12, BLACK);
        }
    }

    // drawGrid(dimensions);
}

void Graphics::drawTileMap(const TileMap &grid, Vector2 dimensions)
{
    if (dimensions.x <= 0 || dimensions.y <= 0)
    {
        return;
    }

    Vector2 tileSize = {_screenDimensions.x / dimensions.x, _screenDimensions.y / dimensions.y};
    const auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            const Tile &tile = tiles[y][x];
            const TileDefinition &sprite = tileSet.getTile(tile.tileId);

            Rectangle destRec = {x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y};

            DrawTexturePro(tileSet.getTexture(), sprite.source, destRec, {0, 0}, 0, WHITE);
        }
    }
}
