#pragma once
#include "game.h"
#include "neighborhood.h"
#include "button.h"
#include "rules.h"

class Graphics
{

private:
    float screenWidth;
    float screenHeight;

    Game *game;
    Neighborhood *neighborhoodArray;
    std::vector<Button> *buttons;

    Rectangle buttonRect;

    Rectangle gameRect;

    Rules *rules;
    Rectangle rulesRect;
    Rectangle *rulesSectionRects;

    Rectangle neighborhoodRectArr[2];
    Rectangle neighborhoodsScreen; // Texture area in which both neighborhoods preside
    int neighborhoodSize;

    int numCellsWidth; // Simulation cells
    int numCellsHeight;
    int cellSize;

    RenderTexture2D gameTexture;
    RenderTexture2D neighborhoodTexture;

    RenderTexture2D rulesTexture;

    Texture2D buttonTexture;
    SpriteInfo buttonSpriteInfo[BUTTON_TYPE_COUNT];

    Texture2D backgroundTexture;

    Color colorPallete[COLOR_COUNT];
    Font myFont;

    // Functions
    void drawSimulation(std::vector<CellUpdate> *dirtyCells);
    void drawNeighborhoods();
    void drawButtons();
    void drawRules();

    void initButSpriteInfo();
    Rectangle recToSourceRec(Rectangle rec);
    Color hexToRgb(std::string hexCode, int opacity = 255);

public:
    Graphics(float screenWidth, float screenHeight, Game *game, Neighborhood *neighborhood, std::vector<Button> *buttons, Rectangle *rulesSecRecs, Rectangle rulesRect);
    ~Graphics();

    void drawGame(int state);
};