#pragma once

#include "ship.h"
#include "playerShip.h"

enum class FollowType
{
    Touch,
    TouchRandom,
    Approach,
};

class EnemyShip : public Ship
{
public:
    EnemyShip(const ShipData &data, World &world, FollowType type,
              PlayerShip &playerShip, const ShipVisuals &visuals,
              AudioManager &audioManager,
              ProjectileSpawner projectileSpawner);

    void update(float delta) override;

    // add health bar to render
    void render() override
    {
        const int barWidth = 60;
        const int barHeight = 10;

        const float healthRatio = data.maxHealth > 0.0f
                                      ? data.health / data.maxHealth
                                      : 0.0f;

        Rectangle bar;
        Rectangle body = Ship::getBody();
        int barVerticalBuffer = 2;
        int outlineWidth = 1;

        bar.x = (body.x + body.width / 2) - barWidth / 2;
        bar.y = body.y - barVerticalBuffer - barHeight;
        bar.width = barWidth;
        bar.height = barHeight;

        DrawRectangleRec(bar, DARKGRAY);
        DrawRectangle(bar.x, bar.y, barWidth * healthRatio, bar.height, RED);
        DrawRectangleLinesEx(bar, outlineWidth, WHITE);

        Ship::render();
    }

private:
    void applyFollowForce();
    void basicFollow(float &xForce, float &yForce);

    FollowType followType;
    PlayerShip &playerShip;
};