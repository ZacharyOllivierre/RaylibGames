#include "control.h"

Control::Control(Camera2D *c, EntityManager *em, StructureManager *sm, TileManager *tm)
{
    camera = c;
    entityManager = em;
    structureManager = sm;
    tileManager = tm;
}

void Control::cameraControls()
{
    static Vector2 prevMouseWorld = {0, 0};
    float wheel = GetMouseWheelMove();

    // If mousewheel is moving apply zoom
    if (wheel != 0)
    {
        camera->zoom = expf(logf(camera->zoom) + (wheel * 0.1f));
    }

    // If mouse pressed update previous mouse position
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        prevMouseWorld = GetScreenToWorld2D(GetMousePosition(), *camera);
    }

    // When held apply portion of difference of current position and saved pos to camera
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 currentMouseWorld = GetScreenToWorld2D(GetMousePosition(), *camera);

        Vector2 delta = {
            prevMouseWorld.x - currentMouseWorld.x,
            prevMouseWorld.y - currentMouseWorld.y};

        camera->target.x += delta.x / 1.5;
        camera->target.y += delta.y / 1.5;

        prevMouseWorld = currentMouseWorld;
    }
}

void Control::debugKeyboardControls()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Entity *entity = entityManager->getRandomEntity();

        if (entity != nullptr)
        {
            Vector2 clickPos = GetScreenToWorld2D(GetMousePosition(), *camera);
            entity->moveToPos(clickPos);
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        Entity *entity = entityManager->getRandomEntity();
        if (entity != nullptr)
        {
            entityManager->killEntity(entity);
        }
    }
    if (IsKeyPressed(KEY_H))
    {
        Entity *entity = entityManager->getRandomEntity();
        Tile *tile = tileManager->getRandomTile();
        if (entity && tile)
        {
            structureManager->createStructure(StructureType::HouseType, tile, entity);
        }
    }
}