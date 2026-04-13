#pragma once
#include "raylib.h"
#include <cmath>

#include "entityManager.h"
#include "tileManager.h"
#include "structureManager.h"

class Control
{
private:
    Camera2D *camera;
    EntityManager *entityManager;
    StructureManager *structureManager;
    TileManager *tileManager;

public:
    Control(Camera2D *c, EntityManager *em, StructureManager *sm, TileManager *tm);

    void cameraControls();
    void debugKeyboardControls();
};