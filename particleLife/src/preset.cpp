#include "preset.h"

#include <iomanip>
#include <limits>

Preset::Preset(SimData &simData) : simData(simData)
{
}

void Preset::load(const std::string &presetName)
{
    std::ifstream inFile("presets.txt");

    if (!inFile)
        return;

    std::string name;

    while (inFile >> std::quoted(name))
    {
        if (name == presetName)
        {
            float innateRepulsionArea;
            float innateRepulsion;
            float maxAttractionArea;
            float maxAttraction;
            float maxSpeed;
            float damping;
            float densityLimit;
            int numTypes;

            if (!(inFile >>
                  innateRepulsionArea >>
                  innateRepulsion >>
                  maxAttractionArea >>
                  maxAttraction >>
                  maxSpeed >>
                  damping >>
                  densityLimit >>
                  numTypes) ||
                numTypes < 0)
            {
                return;
            }

            std::vector<std::vector<float>> attraction(
                numTypes, std::vector<float>(numTypes));

            for (int i = 0; i < numTypes; ++i)
            {
                for (int j = 0; j < numTypes; ++j)
                {
                    if (!(inFile >> attraction[i][j]))
                        return;
                }
            }

            simData.innateRepulsionArea = innateRepulsionArea;
            simData.innateRepulsion = innateRepulsion;
            simData.maxAttractionArea = maxAttractionArea;
            simData.maxAttraction = maxAttraction;
            simData.maxSpeed = maxSpeed;
            simData.damping = damping;
            simData.densityLimit = densityLimit;
            simData.numTypes = numTypes;
            simData.attraction = std::move(attraction);

            // continue scanning so the newest duplicate name wins
            continue;
        }

        skipPreset(inFile);
    }
}

void Preset::save(const std::string &name)
{
    if (simData.numTypes < 0 ||
        simData.attraction.size() != static_cast<size_t>(simData.numTypes))
    {
        return;
    }

    for (const auto &row : simData.attraction)
    {
        if (row.size() != static_cast<size_t>(simData.numTypes))
            return;
    }

    std::ofstream outFile("presets.txt", std::ios::app);

    if (!outFile)
    {
        return;
    }

    outFile << std::setprecision(std::numeric_limits<float>::max_digits10);

    // sim data
    outFile << '"' << name << '"';
    outFile << ' ' << simData.innateRepulsionArea;
    outFile << ' ' << simData.innateRepulsion;
    outFile << ' ' << simData.maxAttractionArea;
    outFile << ' ' << simData.maxAttraction;
    outFile << ' ' << simData.maxSpeed;
    outFile << ' ' << simData.damping;
    outFile << ' ' << simData.densityLimit;
    outFile << ' ' << simData.numTypes;

    // matrix
    for (int i = 0; i < simData.numTypes; ++i)
    {
        for (int j = 0; j < simData.numTypes; ++j)
        {
            outFile << ' ' << simData.attraction[i][j];
        }
    }

    outFile << "\n";

    outFile.close();

    // preset names will rebuild upon request
    namesAreDirty = true;
}

void Preset::remove(const std::string &presetName)
{
    std::ifstream inFile("presets.txt");
    if (!inFile)
        return;

    std::ofstream outFile("presets.txt.tmp");
    if (!outFile)
        return;

    std::string name;
    bool removed = false;

    while (inFile >> std::quoted(name))
    {
        float values[7];
        int numTypes;

        if (!(inFile >> values[0] >> values[1] >> values[2] >> values[3] >>
              values[4] >> values[5] >> values[6] >> numTypes) ||
            numTypes < 0)
        {
            outFile.close();
            std::remove("presets.txt.tmp");
            return;
        }

        std::vector<float> attraction(
            static_cast<size_t>(numTypes) * static_cast<size_t>(numTypes));
        for (float &value : attraction)
        {
            if (!(inFile >> value))
            {
                outFile.close();
                std::remove("presets.txt.tmp");
                return;
            }
        }

        if (name == presetName)
        {
            removed = true;
            continue;
        }

        outFile << std::setprecision(std::numeric_limits<float>::max_digits10);
        outFile << std::quoted(name);
        for (float value : values)
            outFile << ' ' << value;
        outFile << ' ' << numTypes;
        for (float value : attraction)
            outFile << ' ' << value;
        outFile << '\n';
    }

    outFile.close();
    inFile.close();

    if (!removed || std::rename("presets.txt.tmp", "presets.txt") != 0)
    {
        std::remove("presets.txt.tmp");
        return;
    }

    namesAreDirty = true;
}

std::vector<const char *> Preset::getPresetNames()
{
    if (namesAreDirty)
    {
        updatePresetNames();
        namesAreDirty = false;
    }

    std::vector<const char *> names;
    for (const auto &name : presetNames)
    {
        names.push_back(name.c_str());
    }
    return names;
}

void Preset::updatePresetNames()
{
    presetNames.clear();

    std::ifstream inFile("presets.txt");

    if (!inFile)
        return;

    std::string name;

    while (inFile >> std::quoted(name))
    {
        presetNames.push_back(name);

        skipPreset(inFile);
    }
}

// TODO hardcoded to skip the 7 simdata values
// if I add or change the amount, needs to be changed here as well
void Preset::skipPreset(std::ifstream &inFile)
{
    float value;

    for (int i = 0; i < 7; ++i)
        inFile >> value;

    int numTypes;
    inFile >> numTypes;

    // attraction matrix
    for (int i = 0; i < numTypes; ++i)
    {
        for (int j = 0; j < numTypes; ++j)
        {
            inFile >> value;
        }
    }
}