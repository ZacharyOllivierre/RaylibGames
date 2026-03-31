#include "main.h"
// #include <iostream>

void initNeighborhoods();
void passObjToButton(std::vector<Button> &buttonVector, Game *game, Neighborhood *neighborhood);
void initButtons(std::vector<Button> &buttonVector);
void initRulesRecs();

int main()
{

    SetTargetFPS(fps);

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Game of Life");

    initNeighborhoods();
    initRulesRecs();
    initButtons(buttonVector);

    Neighborhood neighborhoodArray[numNeighborhoods] = {
        Neighborhood(neighborhoodSize, neighborhoodCellSize, neighScreenArr[0]),
        Neighborhood(neighborhoodSize, neighborhoodCellSize, neighScreenArr[1])};

    Game game(gameScreen, cellNumWidth, cellNumHeight, cellSize, neighborhoodArray);
    Graphics graphics(screenWidth, screenHeight, &game, neighborhoodArray, &buttonVector, rulesSectionRecs, rulesRect);
    Controls control(&game, fps, &simulatedFrameRate, &state, neighborhoodArray, &buttonVector, rulesSectionRecs);

    // Injection of objects into button
    passObjToButton(buttonVector, &game, neighborhoodArray);

    // Game loop
    while (!WindowShouldClose())
    {
        // std::cout << GetFPS() << "\n";

        BeginDrawing();

        switch (state)
        {

        case HOMESCREEN:

            graphics.drawGame(HOMESCREEN);

            control.homescreenControls();

            break;

        case SIMULATION:

            // Draw Game
            graphics.drawGame(SIMULATION);

            // simulate frame rate
            currentFrameNum++;
            if (simulatedFrameRate > 0 && currentFrameNum >= fps / simulatedFrameRate)
            {
                game.updateGame();
                currentFrameNum = 0;
            }

            control.simulationControls();
            break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void initNeighborhoods()
{
    // sets neighborhood screen array with even x spacing
    float neighSpacing = gameScreen.x / (numNeighborhoods + 1);
    float thirdsOffset = 16;

    neighScreenArr[0] = {neighSpacing - neighborhoodScreenSize / 2 - thirdsOffset, 32, neighborhoodScreenSize, neighborhoodScreenSize};
    neighScreenArr[1] = {neighSpacing * 2 - neighborhoodScreenSize / 2 + thirdsOffset, 32, neighborhoodScreenSize, neighborhoodScreenSize};

    // sets neighborhood cell size
    neighborhoodCellSize = neighScreenArr[0].width / static_cast<float>(neighborhoodSize);
}

void passObjToButton(std::vector<Button> &buttonVector, Game *game, Neighborhood *neighborhood)
{
    // For each button check if type has dependency and pass it
    for (int i = 0; i < buttonVector.size(); i++)
    {
        Button *currentButton = &buttonVector[i];
        ButtonTypes buttonType = currentButton->getType();

        // Simulation buttons game injection - simPtr buttons
        if (buttonType >= SIM_START_STOP && buttonType <= SIM_RESET_RANDOM || buttonType >= SIM_PTR_SIZE_UP && buttonType <= SIM_PTR_SIZE_DOWN)
        {
            currentButton->setGamePointer(game);
        }

        // Neighborhood buttons neighborhood injection
        else if (buttonType >= NEIGH_RESET_EMPTY && buttonType <= NEIGH_RESET_RANDOM ||
                 buttonType >= NEIGH_PTR_SIZE_UP && buttonType <= NEIGH_PTR_SIZE_DOWN)
        {
            currentButton->setNeighborhoodPointer(neighborhood);
        }

        // Rules buttons game injection
        else if (buttonType >= RULES_DISPLAY_STEP && buttonType <= RULES_REMOVE_RULE)
        {
            currentButton->setGamePointer(game);
        }
    }
}

void initButtons(std::vector<Button> &buttonVector)
{
    int rX = gameScreen.x;
    int middleX = gameScreen.x / 2;
    int middleY = screenHeight / 2;

    // Button sizes
    float bWidthHalf = buttonWidth / 2;
    float bHeightHalf = buttonHeight / 2;

    float pauseButtonX = middleX - bWidthHalf;

    float simResetSpacing = pauseButtonX / 4;

    float simPtrStartX = pauseButtonX + buttonWidth;

    // Simulation buttons
    buttonVector.push_back(Button({pauseButtonX, middleY - buttonHeight, buttonWidth, buttonHeight}, SIM_START_STOP, -1, &simulatedFrameRate));
    buttonVector.push_back(Button({simResetSpacing - bWidthHalf / 2, middleY - buttonHeight, bWidthHalf, buttonHeight}, SIM_RESET_EMPTY));
    buttonVector.push_back(Button({simResetSpacing * 2 - bWidthHalf / 2, middleY - buttonHeight, bWidthHalf, buttonHeight}, SIM_RESET_FULL));
    buttonVector.push_back(Button({simResetSpacing * 3 - bWidthHalf / 2, middleY - buttonHeight, bWidthHalf, buttonHeight}, SIM_RESET_RANDOM));

    buttonVector.push_back(Button({simPtrStartX + simResetSpacing - bWidthHalf / 2, middleY - buttonHeight, bWidthHalf, buttonHeight}, SIM_PTR_SIZE_UP));
    buttonVector.push_back(Button({simPtrStartX + (simResetSpacing * 3) - bWidthHalf / 2, middleY - buttonHeight, bWidthHalf, buttonHeight}, SIM_PTR_SIZE_DOWN));

    // Neighborhood buttons
    for (int n = 0; n < numNeighborhoods; n++)
    {
        Rectangle nRec = neighScreenArr[n];

        // Bounding box for neigh buttons
        float x;
        float size = nRec.height / 5;

        // Sets x based on which neighborhood
        n ? x = nRec.x + nRec.width : x = nRec.x - bWidthHalf;

        buttonVector.push_back(Button({x, nRec.y, bWidthHalf, size}, NEIGH_PTR_SIZE_UP, n));
        buttonVector.push_back(Button({x, nRec.y + size * 1, bWidthHalf, size}, NEIGH_PTR_SIZE_DOWN, n));

        buttonVector.push_back(Button({x, nRec.y + size * 2, bWidthHalf, size}, NEIGH_RESET_EMPTY, n));
        buttonVector.push_back(Button({x, nRec.y + size * 3, bWidthHalf, size}, NEIGH_RESET_FULL, n));
        buttonVector.push_back(Button({x, nRec.y + size * 4, bWidthHalf, size}, NEIGH_RESET_RANDOM, n));
    }

    float rulesMiddleY = rulesRect.y + rulesRect.height / 2;
    float secVertSpace = rulesRect.height / 5;

    float ruleBoxTopEdge = rulesSectionRecs[1].y;
    float ruleBoxBotEdge = rulesSectionRecs[2].y + rulesSectionRecs[2].height;
    float ruleBoxRightEdge = rulesSectionRecs[4].x + rulesSectionRecs[4].width;

    float scrollHeight = ruleBoxBotEdge - ruleBoxTopEdge;

    float displayScrollX = (rulesSectionRecs[1].x / 2) - bWidthHalf / 2;
    float rulesStepX = (ruleBoxRightEdge + gameScreen.x) / 2 - bWidthHalf / 2;

    // Rules buttons
    buttonVector.push_back(Button({displayScrollX, rulesMiddleY - scrollHeight / 2, bWidthHalf, scrollHeight}, RULES_DISPLAY_STEP));
    buttonVector.push_back(Button({rulesStepX, rulesMiddleY - scrollHeight / 2, bWidthHalf, scrollHeight}, RULES_SECTION_STEP));

    buttonVector.push_back(Button({rulesSectionRecs[2].x, rulesSectionRecs[2].y + secVertSpace, buttonWidth * 2, bWidthHalf}, RULES_ADD_RULE));
    buttonVector.push_back(Button({rulesSectionRecs[3].x, rulesSectionRecs[3].y + secVertSpace, buttonWidth * 2, bWidthHalf}, RULES_REMOVE_RULE));
}

// Initiallizes rule sections rectangles - coords relative to texture
void initRulesRecs()
{
    // Set bounding box for rules display
    rulesRect = {0, screenHeight / 2, gameScreen.x, screenHeight / 2};

    // Set rule number rec
    float rulesNumWidth = buttonWidth * 3;

    float rulesWidth = buttonWidth * 2;
    float rulesHeight = buttonHeight / 1.5;

    float secVertSpace = rulesRect.height / 5;
    float secHorSpace = rulesRect.width / 10;

    rulesSectionRecs[0] = {
        rulesRect.x + rulesRect.width / 2 - rulesNumWidth / 2,
        rulesRect.y + secVertSpace - rulesHeight / 2,
        rulesNumWidth,
        rulesHeight};
    rulesSectionRecs[1] = {
        rulesRect.x + secHorSpace * 3 - rulesWidth / 2,
        rulesRect.y + secVertSpace * 2 - rulesHeight / 2,
        rulesWidth,
        rulesHeight};
    rulesSectionRecs[4] = {
        rulesRect.x + secHorSpace * 7 - rulesWidth / 2,
        rulesRect.y + secVertSpace * 2 - rulesHeight / 2,
        rulesWidth,
        rulesHeight};
    rulesSectionRecs[2] = {
        rulesRect.x + secHorSpace * 3 - rulesWidth / 2,
        rulesRect.y + secVertSpace * 3 - rulesHeight / 2,
        rulesWidth,
        rulesHeight};
    rulesSectionRecs[3] = {
        rulesRect.x + secHorSpace * 7 - rulesWidth / 2,
        rulesRect.y + secVertSpace * 3 - rulesHeight / 2,
        rulesWidth,
        rulesHeight};
}