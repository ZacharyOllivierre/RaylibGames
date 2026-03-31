#pragma once
#include "game.h"
#include "types.h"
#include "neighborhood.h"
#include "button.h"
#include "rules.h"

class Controls
{

private:
    Game *game;
    Rectangle gameScreen;
    int simCellSize;
    int cellNumWidth;
    int cellNumHeight;

    int simClickRadius;

    Neighborhood *neighborhoodArray;
    Rectangle neighScreenOne;
    Rectangle neighScreenTwo;

    std::vector<Button> *buttons;

    GameState *state;

    Rules *rules;
    Rectangle *sectionScreens;

    int fps;
    int *simulatedFrameRate;
    int storedFrameRate; // for pause

    void constantSimControls();

public:
    Controls(Game *game, int fps, int *simulatedFrameRate, GameState *state, Neighborhood *neigh, std::vector<Button> *buttons, Rectangle *sectionScreens);

    void homescreenControls();
    void simulationControls();
};