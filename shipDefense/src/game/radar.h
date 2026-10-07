#pragma once

#include "raylib.h"
#include <vector>

#include "radarData.h"

// Radar is just formatted data of enemies around player
// Consumed by graphics to output radar on map
// Everything stored as world coordinates

class Radar
{
public:
    Radar(RadarData data) : data(data), idle(false)
    {
        // extra safe set | gets 0s and clears
        updateData(data);
    }

    const RadarData &getData()
    {
        return data;
    }

    void updateScanning(float delta)
    {
        if (idle)
        {
            data.currentIdleTime += delta;

            if (data.currentIdleTime < data.scanningIdleTime)
                return;

            data.currentIdleTime = 0;
            idle = false;
        }

        if (data.currentRadius >= data.maxRadius)
        {
            data.currentRadius = 0;
            idle = true;
            return;
        }
        else
        {
            data.currentRadius += data.scanningSpeed;
        }
    }

    void addActiveShip(Vector2 position)
    {
        data.activeShipPositions.push_back(position);
    }

    void clearActiveShips() { data.activeShipPositions.clear(); }

    void updateData(RadarData data)
    {
        this->data = data;
        data.currentIdleTime = 0;
        data.currentRadius = 0;
        this->data.activeShipPositions.clear();
    }

private:
    RadarData data;

    bool idle;
};