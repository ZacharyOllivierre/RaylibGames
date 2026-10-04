#pragma once

#include "raylib.h"
#include "atlas.h"

class TextureManager
{
public:
    TextureManager()
        : playerBase(LoadTexture("assets/textures/ships/Base/Player - Base - Full health.png")),
          bomberBase(LoadTexture("assets/textures/ships/Base/Enemy - Bomber - Base.png")),
          fighterBase(LoadTexture("assets/textures/ships/Base/Enemy - Fighter - Base.png")),
          playerEngineTexture(LoadTexture("assets/textures/ships/Engine Effect/Player - Engines - Base Engine - Idle-Sheet.png")),
          bomberEngineTexture(LoadTexture("assets/textures/ships/Engine Effect/Enemy - Bomber - Engine-Sheet.png")),
          fighterEngineTexture(LoadTexture("assets/textures/ships/Engine Effect/Enemy - Fighter - Engine-Sheet.png")),
          bomberDestructionTexture(LoadTexture("assets/textures/ships/Destruction/Enemy - Bomber - Destruction-Sheet.png")),
          fighterDestructionTexture(LoadTexture("assets/textures/ships/Destruction/Enemy - Fighter - Destruction-Sheet.png")),
          playerDestructionTexture(LoadTexture("assets/textures/ships/Destruction/Player - Destruction-Sheet.png")),
          pelletTexture(LoadTexture("assets/textures/projectiles/Pellet Bullet-Sheet.png")),
          starBackground(LoadTexture("assets/textures/stars.png")),
          starEmpty(LoadTexture("assets/textures/starsNoBackground.png")),
          playerEngines(playerEngineTexture, {48, 48}),
          bomberEngines(bomberEngineTexture, {64, 64}),
          fighterEngines(fighterEngineTexture, {64, 64}),
          bomberDestruction(bomberDestructionTexture, {64, 64}),
          fighterDestruction(fighterDestructionTexture, {64, 64}),
          playerDestruction(playerDestructionTexture, {48, 48}),
          pelletAtlas(pelletTexture, {8, 16}),
          playerVisuals{playerBase, playerEngines, playerDestruction},
          bomberVisuals{bomberBase, bomberEngines, bomberDestruction},
          fighterVisuals{fighterBase, fighterEngines, fighterDestruction}
    {
    }

    ~TextureManager()
    {
        UnloadTexture(playerBase);
        UnloadTexture(bomberBase);
        UnloadTexture(fighterBase);
        UnloadTexture(playerEngineTexture);
        UnloadTexture(bomberEngineTexture);
        UnloadTexture(fighterEngineTexture);
        UnloadTexture(bomberDestructionTexture);
        UnloadTexture(fighterDestructionTexture);
        UnloadTexture(playerDestructionTexture);
        UnloadTexture(pelletTexture);
        UnloadTexture(starBackground);
        UnloadTexture(starEmpty);
    }

public:
    Texture2D playerBase;
    Texture2D bomberBase;
    Texture2D fighterBase;

    Texture2D playerEngineTexture;
    Texture2D bomberEngineTexture;
    Texture2D fighterEngineTexture;

    Texture2D bomberDestructionTexture;
    Texture2D fighterDestructionTexture;
    Texture2D playerDestructionTexture;
    Texture2D pelletTexture;

    Texture2D starBackground;
    Texture2D starEmpty;

    Atlas playerEngines;
    Atlas bomberEngines;
    Atlas fighterEngines;

    Atlas bomberDestruction;
    Atlas fighterDestruction;
    Atlas playerDestruction;
    Atlas pelletAtlas;

    ShipVisuals playerVisuals;
    ShipVisuals bomberVisuals;
    ShipVisuals fighterVisuals;
};