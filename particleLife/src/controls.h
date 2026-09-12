#pragma once

#include "simulation.h"
#include "simulationTypes.h"

// add to numpad controls if adding clicktype
enum class ClickType
{
    Select,
    Add,
    Remove,
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
    Controls(Simulation *sim, ParticleConfig startingConfig);

    void runControls();

    inline ParticleConfig &getParticleConfig() { return particleConfig; }
    inline ControlData &getControlData() { return data; }
    Camera2D &getCamera() { return camera; }

private:
    void mouseControls(const Vector2 &mousePos);
    void cameraControls();
    void numpadControls();

private:
    Simulation *simulation;
    ParticleConfig particleConfig;

    ControlData data;

    Camera2D camera = {0, 0};
};