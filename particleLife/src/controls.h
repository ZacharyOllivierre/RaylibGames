#pragma once

#include "simulation.h"
#include "simulationTypes.h"

enum class ClickType
{
    Add,
    Remove,
    Select
};

struct ControlData
{
    ClickType clickType = ClickType::Select;
    int addAmount = 1;
    int removeRadius = 10;
};

class Controls
{
public:
    Controls(Simulation *sim, ParticleConfig startingConfig)
        : simulation(sim), particleConfig(startingConfig) {}

    void runControls();

    inline ParticleConfig &getParticleConfig() { return particleConfig; }
    inline ControlData &getControlData() { return data; }

private:
    void mouseControls(const Vector2 &mousePos);

private:
    Simulation *simulation;
    ParticleConfig particleConfig;

    ControlData data;
};