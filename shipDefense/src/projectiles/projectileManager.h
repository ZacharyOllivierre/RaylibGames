#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include "projectile.h"
#include "../collisionManager.h"
#include "../renderManager.h"
#include "../updateManager.h"

class ProjectileManager
{
public:
    ProjectileManager(RenderManager &renderManager,
                      UpdateManager &updateManager,
                      CollisionManager &collisionManager)
        : renderManager(renderManager),
          updateManager(updateManager),
          collisionManager(collisionManager)
    {
    }

    void add(std::unique_ptr<Projectile> projectile)
    {
        Projectile *projectilePointer = projectile.get();
        projectiles.push_back(std::move(projectile));

        updateManager.add(projectilePointer);
        renderManager.add(projectilePointer);
        collisionManager.add(projectilePointer);
    }

    void cleanup()
    {
        for (auto projectile = projectiles.begin();
             projectile != projectiles.end();)
        {
            if (!(*projectile)->isExpired())
            {
                ++projectile;
                continue;
            }

            remove(projectile->get());
            projectile = projectiles.erase(projectile);
        }
    }

    void clear()
    {
        for (const auto &projectile : projectiles)
            remove(projectile.get());

        projectiles.clear();
    }

    std::function<void(std::unique_ptr<Projectile>)> getSpawner()
    {
        return [this](std::unique_ptr<Projectile> projectile)
        {
            add(std::move(projectile));
        };
    }

private:
    void remove(Projectile *projectile)
    {
        renderManager.remove(projectile);
        updateManager.remove(projectile);
        collisionManager.remove(projectile);
    }

    RenderManager &renderManager;
    UpdateManager &updateManager;
    CollisionManager &collisionManager;
    std::vector<std::unique_ptr<Projectile>> projectiles;
};