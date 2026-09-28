#pragma once

#include <memory>

#include "interfaces/updateable.h"

#include "playerShip.h"
#include "sprite.h"
#include "renderManager.h"
#include "updateManager.h"
#include "graphics.h"

// TODO
class Game : public Updateable
{
public:
    // TODO should probably own world
    Game(RenderManager *renderManager, UpdateManager *updateManager,
         World *world, Graphics *graphics);

    void update(float delta) override;

private:
    void initPlayerShip();
    void reset();

private:
    RenderManager *renderManager;
    UpdateManager *updateManager;
    World *world;
    Graphics *graphics;

    std::unique_ptr<PlayerShip> ship;
};