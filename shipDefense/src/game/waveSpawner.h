#pragma once
#include "raylib.h"
#include "../ships/shipManager.h"

class World;

enum class Edge
{
    Top,
    Right,
    Bottom,
    Left,
};

class WaveSpawner
{
public:
    WaveSpawner(float difficulty, int maxRounds, ShipManager &shipManager,
                World &world);

    void spawnNextWave();
    bool hasNextWave() const;
    void reset();

    void getRoundInfo(int &currentRound, int &maxRounds) const
    {
        currentRound = this->round;
        maxRounds = this->maxRounds;
    }

private:
    void calculateWaveShips();
    ShipData calculateShipStats(ShipManager::ShipType shipType);

    Vector2 getEnemySpawnLocation() const;

private:
    float difficulty;

    int round;
    int maxRounds;
    ShipManager &shipManager;
    World &world;
};