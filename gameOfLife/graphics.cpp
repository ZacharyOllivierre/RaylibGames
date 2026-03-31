#include "graphics.h"

Graphics::Graphics(float screenWidth, float screenHeight, Game *game, Neighborhood *neighborhood, std::vector<Button> *buttons, Rectangle *rulesSecRecs, Rectangle rulesRect)
{
    // Set screen
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // Set cells
    cellSize = game->getCellSize();
    numCellsWidth = game->getCellNumWidth();
    numCellsHeight = game->getCellNumHeight();

    // Set game
    this->game = game;
    gameRect = game->getGameScreen();

    // Set neighborhoods
    neighborhoodArray = neighborhood;
    neighborhoodRectArr[0] = neighborhoodArray[0].getNeighScreen();
    neighborhoodRectArr[1] = neighborhoodArray[1].getNeighScreen();
    neighborhoodSize = neighborhoodArray[0].getNeighSize();
    neighborhoodsScreen = {0, 0, screenWidth - gameRect.width, (float)(screenHeight * 0.50)};

    // Set buttons
    this->buttons = buttons;
    buttonRect = {0, 0, screenWidth - gameRect.width, screenHeight};

    // Set rules rectangle
    rules = game->getRulesPtr();
    this->rulesRect = rulesRect;
    rulesSectionRects = rulesSecRecs;

    // Set color pallete
    colorPallete[ColorWheel::DEAD_C] = hexToRgb("#305557");
    colorPallete[ColorWheel::SURVIVE_C] = hexToRgb("#7ecd8e");
    colorPallete[ColorWheel::BORN_C] = hexToRgb("#c0e5d5");
    colorPallete[ColorWheel::DIED_C] = hexToRgb("#277c74");
    colorPallete[ColorWheel::NEIGH_C] = hexToRgb("#4a564f");
    colorPallete[ColorWheel::NEIGH_CENTER_C] = hexToRgb("#597d64");
    colorPallete[ColorWheel::NEIGH_SECONDARY_C] = hexToRgb("#bfe6c7");
    colorPallete[ColorWheel::NEIGH_SECONDARY_CENTER_C] = hexToRgb("#7ecd8e");
    colorPallete[ColorWheel::RULES_C] = hexToRgb("#00af97");
    colorPallete[ColorWheel::RULES_SECONDARY_C] = hexToRgb("#bfe6c7");

    // Load textures
    gameTexture = LoadRenderTexture(gameRect.width, gameRect.height);
    neighborhoodTexture = LoadRenderTexture(neighborhoodsScreen.width, neighborhoodsScreen.height);
    rulesTexture = LoadRenderTexture(rulesRect.width, rulesRect.height);

    buttonTexture = LoadTexture("assets/buttonSpriteSheet.png");
    initButSpriteInfo();

    backgroundTexture = LoadTexture("assets/backgroundTexture.png");

    // Load fonts
    myFont = LoadFont("assets/font.ttf");
}

Graphics::~Graphics()
{
    // Unload Textures
    UnloadRenderTexture(gameTexture);
    UnloadRenderTexture(neighborhoodTexture);
    UnloadRenderTexture(rulesTexture);

    UnloadTexture(buttonTexture);
    UnloadTexture(backgroundTexture);

    UnloadFont(myFont);
}

void Graphics::drawGame(int state)
{

    switch (state)
    {

    case HOMESCREEN:

        break;

    case SIMULATION:

        // Update neighborhood texture if its been changed
        for (int n = 0; n < 2; n++)
        {
            if (neighborhoodArray[n].getNeighChanged())
            {
                BeginTextureMode(neighborhoodTexture);
                drawNeighborhoods();
                EndTextureMode();
                neighborhoodArray[n].setNeighChanged(false);
            }
        }

        // Update game texture each frame conditionally
        if (!(game->getDirtyCells()->empty()))
        {
            BeginTextureMode(gameTexture);
            drawSimulation(game->getDirtyCells());
            EndTextureMode();
        }

        // Update rules texture if rules display or value has been changed
        if (rules->getChanged())
        {
            BeginTextureMode(rulesTexture);
            drawRules();
            EndTextureMode();
        }

        // Draw background image
        DrawTexturePro(backgroundTexture, {50, 50, buttonRect.width, buttonRect.height}, buttonRect, {0, 0}, 0, colorPallete[ColorWheel::DEAD_C]);

        // Draw buttons
        drawButtons();

        // Draw rules
        DrawTexturePro(rulesTexture.texture, recToSourceRec(rulesRect), rulesRect, {0, 0}, 0, WHITE);

        // Draw game
        DrawTexturePro(gameTexture.texture, recToSourceRec(gameRect), gameRect, {0, 0}, 0, WHITE);

        // Draw neighborhood
        DrawTexturePro(neighborhoodTexture.texture, recToSourceRec(neighborhoodsScreen), neighborhoodsScreen, {0, 0}, 0, WHITE);
    }
}

// private
void Graphics::drawSimulation(std::vector<CellUpdate> *dirtyCells)
{
    for (int i = 0; i < dirtyCells->size(); i++)
    {
        DrawRectangle((*dirtyCells)[i].col * cellSize, (*dirtyCells)[i].row * cellSize, cellSize, cellSize, colorPallete[(*dirtyCells)[i].state]);
    }

    // clear dirty cells
    dirtyCells->clear();
}

void Graphics::drawNeighborhoods()
{
    // calc cell dimentions
    int cellWidth = neighborhoodRectArr[0].width / neighborhoodSize;
    int cellHeight = neighborhoodRectArr[0].height / neighborhoodSize;

    float offset = (neighborhoodRectArr[0].width - neighborhoodSize * cellWidth) / 2.0;

    int numNeighborhoods = sizeof(neighborhoodRectArr) / sizeof(neighborhoodRectArr[0]);

    // For each neighborhood
    for (int n = 0; n < numNeighborhoods; n++)
    {
        // For every cell
        for (int y = 0; y < neighborhoodSize; y++)
        {
            for (int x = 0; x < neighborhoodSize; x++)
            {
                // assume state color is off
                Color squareColor = colorPallete[ColorWheel::NEIGH_C];

                // Calculate pos of square
                int posX = x * cellWidth + (neighborhoodRectArr[n].x);
                int posY = y * cellHeight + (neighborhoodRectArr[n].y);

                // if square is on change color
                if (neighborhoodArray[n].getNeighArrayRowCol(y, x))
                {
                    squareColor = colorPallete[ColorWheel::NEIGH_SECONDARY_C];

                    // if square is center
                    if (y == neighborhoodSize / 2 && x == neighborhoodSize / 2)
                    {
                        squareColor = colorPallete[ColorWheel::NEIGH_SECONDARY_CENTER_C];
                    }
                }
                else
                {
                    if (y == neighborhoodSize / 2 && x == neighborhoodSize / 2)
                    {
                        squareColor = colorPallete[ColorWheel::NEIGH_CENTER_C];
                    }
                }

                DrawRectangle(posX + offset, posY + offset, cellWidth, cellHeight, squareColor);
            }
        }
    }
}

void Graphics::drawButtons()
{
    // For all buttons
    for (int i = 0; i < buttons->size(); i++)
    {
        Rectangle rec = *(*buttons)[i].getButtonScreen();
        ButtonTypes type = (*buttons)[i].getType();

        if (buttonSpriteInfo[type].width == 0 || buttonSpriteInfo[type].height == 0)
            type = SIM_PTR_SIZE_DOWN;

        // Draw state if clicked
        if ((*buttons)[i].updateClickTime())
        {
            DrawTexturePro(
                buttonTexture,
                {(float)buttonSpriteInfo[type].pressedX,
                 (float)buttonSpriteInfo[type].pressedY,
                 (float)buttonSpriteInfo[type].width,
                 (float)buttonSpriteInfo[type].height},
                rec,
                {0, 0},
                0,
                WHITE);
        }
        else
        {
            DrawTexturePro(
                buttonTexture,
                {(float)buttonSpriteInfo[type].x,
                 (float)buttonSpriteInfo[type].y,
                 (float)buttonSpriteInfo[type].width,
                 (float)buttonSpriteInfo[type].height},
                rec,
                {0, 0},
                0,
                WHITE);
        }
    }
}

void Graphics::drawRules()
{
    int fontSize = 33;
    int fontSpacing = 2;

    CellRule cellRule;
    int ruleNum;
    rules->getDisplayedRule(cellRule, ruleNum);

    // 4 sections: rule num, neighborhood, min, max, output
    const int numSections = RuleSections::RULE_SECTION_COUNT;
    std::string sectionName[] = {"Rule: ", "Nei: ", "Min: ", "Max: ", "Output: "};

    // for each section
    for (int i = 0; i < numSections; i++)
    {
        std::string text = sectionName[i];

        // Switch text based on currect section (i)
        switch (i)
        {
        case 0:
            text += (std::to_string(ruleNum + 1));
            break;
        case 1:
            text += (std::to_string(cellRule.neighAffiliation + 1));
            break;
        case 2:
            text += std::to_string(cellRule.min).substr(0, 4);
            break;
        case 3:
            text += (std::to_string(cellRule.max)).substr(0, 4);
            break;
        case 4:
            text += (std::to_string(cellRule.output));
            break;
        default:
            break;
        }

        Rectangle rec = {rulesSectionRects[i].x - rulesRect.x, rulesSectionRects[i].y - rulesRect.y, rulesSectionRects[i].width, rulesSectionRects[i].height};

        // Draw boxes for text
        DrawRectangleRec(rec, colorPallete[ColorWheel::RULES_C]);

        DrawTextEx(
            myFont,
            text.c_str(),
            {rec.x + rec.width / 2 - MeasureTextEx(myFont, text.c_str(), fontSize, fontSpacing).x / 2,
             rec.y + rec.height / 2 - fontSize / 2},
            fontSize,
            fontSpacing,
            colorPallete[ColorWheel::RULES_SECONDARY_C]);
    }

    rules->setChanged(false);
}

void Graphics::initButSpriteInfo()
{
    // Pressed button sprites are assumed to be directly below
    float simButtonStartY = 0;
    float simButtonWidth = 48;
    float simButtonHeight = 96;

    // 4 half width one full width
    buttonSpriteInfo[SIM_START_STOP] = {240, 0, simButtonWidth * 2, simButtonWidth * 2, 240, simButtonWidth * 2};
    buttonSpriteInfo[SIM_RESET_EMPTY] = {simButtonWidth * 0, simButtonStartY, simButtonWidth, simButtonHeight, simButtonWidth * 0, simButtonHeight};
    buttonSpriteInfo[SIM_RESET_FULL] = {simButtonWidth * 1, simButtonStartY, simButtonWidth, simButtonHeight, simButtonWidth * 1, simButtonHeight};
    buttonSpriteInfo[SIM_RESET_RANDOM] = {simButtonWidth * 2, simButtonStartY, simButtonWidth, simButtonHeight, simButtonWidth * 2, simButtonHeight};
    buttonSpriteInfo[SIM_PTR_SIZE_UP] = {simButtonWidth * 3, simButtonStartY, simButtonWidth, simButtonHeight, simButtonWidth * 3, simButtonHeight};
    buttonSpriteInfo[SIM_PTR_SIZE_DOWN] = {simButtonWidth * 4, simButtonStartY, simButtonWidth, simButtonHeight, simButtonWidth * 4, simButtonHeight};

    float nStartY = simButtonHeight * 2;
    float neighButWidth = 48;
    float neighButHeight = 35;
    buttonSpriteInfo[NEIGH_PTR_SIZE_UP] = {0, nStartY, neighButWidth, neighButHeight, neighButWidth, nStartY};
    buttonSpriteInfo[NEIGH_PTR_SIZE_DOWN] = {0, nStartY + neighButHeight * 1, neighButWidth, neighButHeight, neighButWidth, nStartY + neighButHeight * 1};
    buttonSpriteInfo[NEIGH_RESET_EMPTY] = {0, nStartY + neighButHeight * 2, neighButWidth, neighButHeight, neighButWidth, nStartY + neighButHeight * 2};
    buttonSpriteInfo[NEIGH_RESET_FULL] = {0, nStartY + neighButHeight * 3, neighButWidth, neighButHeight, neighButWidth, nStartY + neighButHeight * 3};
    buttonSpriteInfo[NEIGH_RESET_RANDOM] = {0, nStartY + neighButHeight * 4, neighButWidth, neighButHeight, neighButWidth, nStartY + neighButHeight * 4};

    float rStartY = simButtonHeight * 2 + neighButHeight * 5;
    float rWidth = 48;
    float rHeight = 140;

    buttonSpriteInfo[RULES_DISPLAY_STEP] = {0, rStartY, rWidth, rHeight, rWidth, rStartY};
    buttonSpriteInfo[RULES_SECTION_STEP] = {0, rStartY + rHeight, rWidth, rHeight, rWidth, rStartY + rHeight};

    rStartY += rHeight * 2;
    rWidth = 192;
    rHeight = 48;

    buttonSpriteInfo[RULES_ADD_RULE] = {0, rStartY, rWidth, rHeight, rWidth, rStartY};
    buttonSpriteInfo[RULES_REMOVE_RULE] = {0, rStartY + rHeight, rWidth, rHeight, rWidth, rStartY + rHeight};
}

Rectangle Graphics::recToSourceRec(Rectangle rec)
{
    return {0, 0, rec.width, -rec.height};
}

Color Graphics::hexToRgb(std::string hexCode, int opacity)
{
    unsigned char r, g, b;
    std::string cleanHexCode;

    if (hexCode[0] == '#')
    {
        cleanHexCode = hexCode.substr(1);
    }
    else
    {
        cleanHexCode = hexCode;
    }

    r = std::stoi(cleanHexCode.substr(0, 2), nullptr, 16);
    g = std::stoi(cleanHexCode.substr(2, 2), nullptr, 16);
    b = std::stoi(cleanHexCode.substr(4, 2), nullptr, 16);

    return Color{r, g, b, (unsigned char)opacity};
}