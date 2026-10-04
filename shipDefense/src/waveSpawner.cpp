#include "waveSpawner.h"

#include "world.h"

WaveSpawner::WaveSpawner(float difficulty, int maxRounds,
                         ShipManager &shipManager, World &world)
    : difficulty(difficulty), round(0), maxRounds(maxRounds),
      shipManager(shipManager), world(world)
{
}

void WaveSpawner::spawnNextWave()
{
    if (round >= maxRounds)
        return;

    ++round;
    calculateWaveShips();
}

bool WaveSpawner::hasNextWave() const
{
    return round < maxRounds;
}

void WaveSpawner::reset()
{
    round = 0;
}

// TODO system not really compatable with adding more ship types
void WaveSpawner::calculateWaveShips()
{
    int bomberCost = 5;
    int gunnerCost = 10;

    int money = round * 10;

    // randomly buys either bomber or gunner
    while (money >= 0)
    {
        int shipType = GetRandomValue(0, 1);

        if (shipType == 0)
        {
            money -= bomberCost;

            shipManager.createShip(ShipManager::ShipType::Bomber,
                                   calculateShipStats(ShipManager::ShipType::Bomber));
        }
        else if (shipType == 1)
        {
            money -= gunnerCost;

            shipManager.createShip(ShipManager::ShipType::Gunner,
                                   calculateShipStats(ShipManager::ShipType::Gunner));
        }
    }
}

ShipData WaveSpawner::calculateShipStats(ShipManager::ShipType shipType)
{
    const float progression = static_cast<float>(round) * difficulty;
    ShipData data;

    if (shipType == ShipManager::ShipType::Bomber)
    {
        data.startingPosition = getEnemySpawnLocation();
        data.hitBoxSize = {60, 60};
        data.textureSize = {90, 90};
        data.speed = 500.0f + (progression * 60);
        data.health = 50.0f + (progression * 8);
        data.contactDamage = 0.5f;
        data.wrapAtEdges = true;
    }
    else if (shipType == ShipManager::ShipType::Gunner)
    {
        data.startingPosition = getEnemySpawnLocation();
        data.hitBoxSize = {60, 60};
        data.textureSize = {90, 90};
        data.speed = 400.0f + (progression * 60);
        data.health = 100.0f + (progression * 8);
        data.contactDamage = 0.6f;
        data.wrapAtEdges = true;
    }

    return data;
}

Vector2 WaveSpawner::getEnemySpawnLocation() const
{
    const Edge wall = static_cast<Edge>(GetRandomValue(0, 3));
    const Vector2 dimensions = world.getDimensions();

    if (wall == Edge::Top)
        return {(float)GetRandomValue(0, dimensions.x), 0};

    if (wall == Edge::Right)
        return {dimensions.x, (float)GetRandomValue(0, dimensions.y)};

    if (wall == Edge::Bottom)
        return {(float)GetRandomValue(0, dimensions.x), dimensions.y};

    return {0, (float)GetRandomValue(0, dimensions.y)};
}