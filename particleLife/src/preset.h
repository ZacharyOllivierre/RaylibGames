#pragma once
#include "simulationTypes.h"

#include <fstream>

// TODO probably doesn't need to be a class
class Preset
{
public:
    Preset(SimData &simData);

    void load(const std::string &name);
    void save(const std::string &name);
    void remove(const std::string &name);

    std::vector<const char *> getPresetNames();
    bool getIfDirty() { return namesAreDirty; }

private:
    void skipPreset(std::ifstream &inFile);
    void updatePresetNames();

private:
    SimData &simData;
    std::vector<std::string> presetNames;
    bool namesAreDirty = true;
};