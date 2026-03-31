#pragma once
#include "game.h"
#include "neighborhood.h"
#include "types.h"
#include "rules.h"

class Button
{
private:
    Rectangle buttonScreen;
    ButtonTypes type;

    int textValue;

    Game *game;
    Neighborhood *neighborhood;
    Rules *rules;

    int parentNeighborhood; // Neighborhood affiliation | -1 if none

    int *simulatedFrameRate;
    int lastFrameRate;

    // flag for button to be printed down, reset by graphics
    bool clicked;
    int clickLength;
    int curClickTime;

    // functions
    int regionOfButtonVert(Vector2 mousePos, int numRegions);
    int regionOfButtonHor(Vector2 mousePos, int numRegions);

    void pauseButton();
    void rulesDisplayStep(int region);
    void rulesSectionStep(int region);
    void rulesAddRule(int region);

public:
    Button(Rectangle buttonScreen, ButtonTypes type, int parentNeighborhood = -1, int *simulatedFrameRate = nullptr);

    void clickButton(Vector2 mousePos);

    bool updateClickTime();

    // Getter
    inline Rectangle *getButtonScreen() { return &buttonScreen; }
    inline ButtonTypes getType() { return type; }

    // Setters
    void setNeighborhoodPointer(Neighborhood *neighborhood);
    void setGamePointer(Game *game);
};
