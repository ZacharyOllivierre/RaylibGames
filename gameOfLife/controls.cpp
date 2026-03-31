#include "controls.h"

Controls::Controls(Game *game, int fps, int *simulatedFrameRate, GameState *state, Neighborhood *neigh, std::vector<Button> *buttons, Rectangle *sectionScreens)
{
    // Set game
    this->game = game;
    gameScreen = game->getGameScreen();
    simCellSize = game->getCellSize();

    cellNumHeight = game->getCellNumHeight();
    cellNumWidth = game->getCellNumWidth();

    // Set neighborhoods
    neighborhoodArray = neigh;
    neighScreenOne = neighborhoodArray[0].getNeighScreen();
    neighScreenTwo = neighborhoodArray[1].getNeighScreen();

    // Set buttons
    this->buttons = buttons;

    // Set miscalanous - holy spelling
    this->fps = fps;
    this->simulatedFrameRate = simulatedFrameRate;
    this->state = state;
    storedFrameRate = 1;

    rules = game->getRulesPtr();
    this->sectionScreens = sectionScreens;
}

void Controls::homescreenControls()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        *state = SIMULATION;
    }
}

void Controls::simulationControls()
{
    Vector2 mousePosition = GetMousePosition();

    // Check constant controls
    constantSimControls();

    // Conditional control checks - based on mouse position
    // If in simulation
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, gameScreen))
    {
        game->clickSimCell(mousePosition);
    }

    // Check neighborhood clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, neighScreenOne))
    {
        neighborhoodArray[0].clickNeighCell(mousePosition);
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, neighScreenTwo))
    {
        neighborhoodArray[1].clickNeighCell(mousePosition);
    }

    // Buttons
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (int i = 0; i < buttons->size(); i++)
        {
            if (CheckCollisionPointRec(mousePosition, *(*buttons)[i].getButtonScreen()))
            {
                (*buttons)[i].clickButton(mousePosition);
            }
        }
    }

    // Last clicked rule box
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (int i = NEIGHBORHOOD_NUM; i < RULE_SECTION_COUNT; i++)
        {
            if (CheckCollisionPointRec(mousePosition, sectionScreens[i]))
            {
                rules->setLastClickedSection((RuleSections)i);
            }
        }
    }
}

// private
void Controls::constantSimControls()
{
    // Reset button
    if (IsKeyPressed(KEY_R))
    {
        game->resetEmpty();
    }
    if (IsKeyPressed(KEY_T))
    {
        game->resetFull();
    }
    if (IsKeyPressed(KEY_Y))
    {
        game->resetRandom();
    }

    // Pause control
    if (IsKeyPressed(KEY_SPACE))
    {
        if (*simulatedFrameRate > 0)
        {

            storedFrameRate = *simulatedFrameRate;
            *simulatedFrameRate = 0;
        }
        else
        {
            *simulatedFrameRate = storedFrameRate;
        }
    }

    // Framerate control
    if (IsKeyDown(KEY_UP))
    {
        if (*simulatedFrameRate < fps)
            (*simulatedFrameRate) += 1;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        if (*simulatedFrameRate > 1)
            (*simulatedFrameRate) -= 1;
    }
}