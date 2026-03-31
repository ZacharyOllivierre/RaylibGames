#include "game.h"

Game::Game(Rectangle gameScreen, int cellNumWidth, int cellNumHeight, int cellSize, Neighborhood *neigh)
{
    // set values
    this->gameScreen = gameScreen;
    this->cellNumWidth = cellNumWidth;
    this->cellNumHeight = cellNumHeight;
    this->cellSize = cellSize;

    simClickRadius = 1;

    numNeighborhoods = 2;
    neighborhoodArray = neigh;

    numCellNeighborsPtr = new float[numNeighborhoods];

    // set buffer / pointers to
    bufferA = std::vector<std::vector<int>>(cellNumHeight, std::vector<int>(cellNumWidth, 0));
    bufferB = std::vector<std::vector<int>>(cellNumHeight, std::vector<int>(cellNumWidth, 0));
    currentFrame = &bufferA;
    nextFrame = &bufferB;

    // Set all states to 0
    resetEmpty();

    // Set default rules
    rules.addRule(0.210, 0.220, 1, 0);
    rules.addRule(0.350, 0.500, 0, 0);
    rules.addRule(0.750, 0.850, 0, 0);
    rules.addRule(0.100, 0.280, 0, 1);
    rules.addRule(0.430, 0.550, 1, 1);
    rules.addRule(0.120, 0.150, 0, 0);
}

Game::~Game()
{
    delete[] numCellNeighborsPtr;
}

void Game::updateGame()
{

    CellEvent cellState;

    // Update all cells
    for (int y = 0; y < cellNumHeight; y++)
    {
        for (int x = 0; x < cellNumWidth; x++)
        {
            cellState = (CellEvent)getCellState(y, x);

            // For each neighborhood
            for (int n = 0; n < numNeighborhoods; n++)
            {
                // calculate num neighbors
                numCellNeighborsPtr[n] = calcCellNeighbors(x, y, &neighborhoodArray[n]);
            }

            // update state by neighborhood rules
            cellState = updateCell(cellState);

            // specify state, born, survived, died, dead
            cellState = updateCellType(cellState, (CellEvent)(*currentFrame)[y][x]);

            (*nextFrame)[y][x] = cellState;

            // Check if cell has been changed
            if ((*currentFrame)[y][x] != (*nextFrame)[y][x])
            {
                dirtyCells.push_back({y, x, static_cast<CellEvent>((*nextFrame)[y][x])});
            }
        }
    }
    std::swap(currentFrame, nextFrame);
}

void Game::resetEmpty()
{
    for (int y = 0; y < cellNumHeight; y++)
    {

        for (int x = 0; x < cellNumWidth; x++)
        {
            setCell(y, x, DEAD);
        }
    }
}

void Game::resetFull()
{

    for (int y = 0; y < cellNumHeight; y++)
    {

        for (int x = 0; x < cellNumWidth; x++)
        {
            setCell(y, x, ALIVE_BORN);
        }
    }
}

void Game::resetRandom()
{

    for (int y = 0; y < cellNumHeight; y++)
    {
        for (int x = 0; x < cellNumWidth; x++)
        {

            // 25% chance to be on
            int num = GetRandomValue(1, 4);

            if (num == 1)
            {
                setCell(y, x, ALIVE_BORN);
            }
        }
    }
}

bool Game::cellIsAlive(int row, int col)
{
    if ((*currentFrame)[row][col] == ALIVE_SURVIVED || (*currentFrame)[row][col] == ALIVE_BORN)
    {
        return true;
    }
    return false;
}

void Game::flipCellState(int row, int col)
{
    if (cellIsAlive(row, col))
    {
        setCell(row, col, ALIVE_DIED);
    }
    else
    {
        setCell(row, col, ALIVE_BORN);
    }
}

// Flips cell state of all cells within circular simClickRadius
void Game::clickSimCell(Vector2 mousePosition)
{

    int row = (mousePosition.y - gameScreen.y) / cellSize;
    int col = (mousePosition.x - gameScreen.x) / cellSize;

    // for every cell in box of size radius - contains circle
    for (int y = row - simClickRadius; y < row + simClickRadius; y++)
    {

        for (int x = col - simClickRadius; x < col + simClickRadius; x++)
        {
            // If cell to consider is within game grid
            if (y >= 0 && y < cellNumHeight && x >= 0 && x < cellNumWidth)
            {

                int distFromCenterY = y - row;
                int distFromCenterX = x - col;

                // If cell is within circle to draw distX^2 + distY^2 <= radius^2
                if (squareInt(distFromCenterX) + squareInt(distFromCenterY) < squareInt(simClickRadius))
                {
                    flipCellState(y, x);
                }
            }
        }
    }
}

// Increments or Decrements click radius based on bool
void Game::simClickRadiusUp(bool increment)
{
    int sizeStep = 5;

    // If size up or down
    increment ? simClickRadius += sizeStep : simClickRadius -= sizeStep;

    // Bind radius to more than 0 less than larger game size
    if (simClickRadius < 1)
    {
        simClickRadius = 1;
    }
    else if (simClickRadius > cellNumWidth / 2 && simClickRadius > cellNumHeight / 2)
    {
        // set click radius to larger game dimension
        cellNumWidth > cellNumHeight ? simClickRadius = cellNumWidth / 2 : simClickRadius = cellNumHeight / 2;
    }
}

// Getters
int Game::getCellSize() { return cellSize; }
int Game::getCellNumWidth() { return cellNumWidth; }
int Game::getCellNumHeight() { return cellNumHeight; }
int Game::getCellState(int row, int col) { return (*currentFrame)[row][col]; }
std::vector<CellUpdate> *Game::getDirtyCells() { return &dirtyCells; }
Rectangle Game::getGameScreen() { return gameScreen; }
Rules *Game::getRulesPtr() { return &rules; }

// Setters
void Game::setCell(int row, int col, CellEvent state)
{
    (*currentFrame)[row][col] = state;

    dirtyCells.push_back({row, col, state});
}

// Private
float Game::calcCellNeighbors(int x, int y, Neighborhood *neigh)
{
    int numNeighbors = 0;
    int radius = neigh->getNeighSize() / 2;

    std::vector<std::pair<int, int>> *offsets = neigh->getNeighOffsets();
    int neighborhoodSize = offsets->size();

    for (int i = 0; i < offsets->size(); i++)
    {
        // caclulate row and col from offsets from center
        int neighborRow = y + (*offsets)[i].first;
        int neighborCol = x + (*offsets)[i].second;

        // if inside grid
        if (neighborRow >= 0 && neighborRow < cellNumHeight && neighborCol >= 0 && neighborCol < cellNumWidth)
        {
            if (cellIsAlive(neighborRow, neighborCol))
            {
                numNeighbors++;
            }
        }
    }

    return (float)numNeighbors / (float)neighborhoodSize;
}

// returns a cellEvent based on given cellEvent and all neighborhood rules
CellEvent Game::updateCell(CellEvent cellState)
{
    int rulesLength = rules.getCellRulesLength();

    // For each stored rule
    for (int r = 0; r < rulesLength; r++)
    {
        CellRule *rule = rules.getRuleIndex(r);

        int ruleNeighborhood = rule->neighAffiliation;

        if (numCellNeighborsPtr[ruleNeighborhood] >= rule->min &&
            numCellNeighborsPtr[ruleNeighborhood] <= rule->max)
        {
            cellState = (CellEvent)rule->output;
        }
    }

    return cellState;
}

// returns specific CellEvent (born - died) from cellState after rules and "previous" cell state
CellEvent Game::updateCellType(CellEvent cellState, CellEvent frameState)
{
    // ALIVE_DIED always decays to DEAD
    if (frameState == ALIVE_DIED)
        return DEAD;

    // DEAD -> ALIVE = Born
    if ((frameState == DEAD) && cellState == ALIVE_SURVIVED)
        return ALIVE_BORN;

    // ALIVE_BORN -> ALIVE_SURVIVED
    if (frameState == ALIVE_BORN && cellState != DEAD)
        return ALIVE_SURVIVED;

    // ALIVE -> DEAD = Died
    if ((frameState == ALIVE_BORN || frameState == ALIVE_SURVIVED) && cellState == DEAD)
        return ALIVE_DIED;

    return cellState;
}