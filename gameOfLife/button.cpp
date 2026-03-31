#include "button.h"

Button::Button(Rectangle buttonScreen, ButtonTypes type, int parentNeighborhood, int *simulatedFrameRate)
{
    // Set values
    this->buttonScreen = buttonScreen;
    this->type = type;
    this->simulatedFrameRate = simulatedFrameRate;

    textValue = type;

    // number of frames button will print as pressed down
    clickLength = 10;

    // Set stored frame rate if pause button
    if (simulatedFrameRate != nullptr)
    {
        lastFrameRate = *simulatedFrameRate;
    }
    else
    {
        lastFrameRate = -1;
    }

    // Neighborhood affiliation | -1 if none
    this->parentNeighborhood = parentNeighborhood;

    // Set to nullptr for all buttons - updated later if injecting game / neigh
    game = nullptr;
    neighborhood = nullptr;
    rules = nullptr;
}

void Button::clickButton(Vector2 mousePos)
{
    // Start click time and flag
    clicked = true;
    curClickTime = 0;

    switch (type)
    {
    case SIM_START_STOP:
        pauseButton();
        break;

    case SIM_RESET_EMPTY:
        game->resetEmpty();
        break;

    case SIM_RESET_FULL:
        game->resetFull();
        break;

    case SIM_RESET_RANDOM:
        game->resetRandom();
        break;

    case NEIGH_RESET_EMPTY:
        neighborhood[parentNeighborhood].resetEmpty();
        break;

    case NEIGH_RESET_FULL:
        neighborhood[parentNeighborhood].resetFull();
        break;

    case NEIGH_RESET_RANDOM:
        neighborhood[parentNeighborhood].resetRandom();
        break;

    case SIM_PTR_SIZE_UP:
        game->simClickRadiusUp(true);
        break;

    case SIM_PTR_SIZE_DOWN:
        game->simClickRadiusUp(false);
        break;

    case NEIGH_PTR_SIZE_UP:
        neighborhood[parentNeighborhood].neighClickRadiusUp(true);
        break;

    case NEIGH_PTR_SIZE_DOWN:
        neighborhood[parentNeighborhood].neighClickRadiusUp(false);
        break;

    case RULES_DISPLAY_STEP:
        rulesDisplayStep(regionOfButtonVert(mousePos, 2));
        break;

    case RULES_SECTION_STEP:
        rulesSectionStep(regionOfButtonVert(mousePos, 4));
        break;

    case RULES_ADD_RULE:
        rulesAddRule(regionOfButtonHor(mousePos, 2));
        break;

    case RULES_REMOVE_RULE:
        rules->removeDisplayRule();
        break;

    default:
        break;
    }
}

// returns if buttons should still be clicked - updates clicktime if still valid
bool Button::updateClickTime()
{
    if (curClickTime > clickLength)
    {
        clicked = false;
        return false;
    }
    else
    {
        curClickTime++;
        return true;
    }
}

// Getters
// Setters
void Button::setNeighborhoodPointer(Neighborhood *neighborhood) { this->neighborhood = neighborhood; }
void Button::setGamePointer(Game *game)
{
    this->game = game;

    // set local instance of rules pointer if button needs it
    if (type >= RULES_DISPLAY_STEP && type <= RULES_REMOVE_RULE)
    {
        rules = game->getRulesPtr();
    }
}

// private
// Splits button into numRegions vertical sections, returns section mouse is in | Top to bottom
int Button::regionOfButtonVert(Vector2 mousePos, int numRegions)
{
    int sectionHeight = buttonScreen.height / numRegions;

    for (int i = 0; i < numRegions; i++)
    {
        // If mouse resides in current region
        if (mousePos.y >= buttonScreen.y + i * sectionHeight && mousePos.y <= buttonScreen.y + (i * sectionHeight) + sectionHeight)
        {
            return i;
        }
    }

    // Error button clicked but not in any vertical regions
    return -1;
}

// Left to right 0 - numRegions
int Button::regionOfButtonHor(Vector2 mousePos, int numRegions)
{

    int sectionWidth = buttonScreen.width / numRegions;

    for (int i = 0; i < numRegions; i++)
    {
        if (mousePos.x >= buttonScreen.x + i * sectionWidth && mousePos.x <= buttonScreen.x + (i * sectionWidth) + sectionWidth)
        {
            return i;
        }
    }

    return -1;
}

void Button::pauseButton()
{
    if (*simulatedFrameRate > 0)
    {
        lastFrameRate = *simulatedFrameRate;
        *simulatedFrameRate = 0;
    }
    else
    {
        *simulatedFrameRate = lastFrameRate;
    }
}

// Increments or decrements displayed rule
void Button::rulesDisplayStep(int region)
{
    // If mouse position is in top half of button
    if (!region)
    {
        rules->incDisplayedRules();
    }
    else
    {
        rules->decDisplayedRules();
    }
}

// Increments or decrements section value based on region of button clicked
void Button::rulesSectionStep(int region)
{
    int higherStep = 10;
    int lowerStep = 1;

    switch (region)
    {
    case 0: // Top top of button
        rules->adjustLastClicked(higherStep);
        break;
    case 1: // Top bottom of button
        rules->adjustLastClicked(lowerStep);
        break;
    case 2: // Bottom top
        rules->adjustLastClicked(-lowerStep);
        break;
    case 3: // Bottom bottom
        rules->adjustLastClicked(-higherStep);
        break;
    }
}

void Button::rulesAddRule(int region)
{
    if (!region)
    {
        rules->insertRuleAtDisplay();
    }
    else
    {
        rules->insertRuleAfterDisplay();
    }
}