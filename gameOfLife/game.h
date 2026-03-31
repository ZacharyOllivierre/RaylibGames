#pragma once
#include <vector>
#include <raylib.h>
#include "types.h"
#include "neighborhood.h"
#include "rules.h"
#include "util.h"

class Game
{

private:
    Rectangle gameScreen;

    int cellSize;
    int cellNumWidth;
    int cellNumHeight;

    int simClickRadius; // Radius of click area

    int numNeighborhoods;
    Neighborhood *neighborhoodArray;

    float *numCellNeighborsPtr; // Array to hold averages of neigh size to active neighbors

    Rules rules; // holds rules for updates

    // package of dirty cells to redrawn by graphics
    std::vector<CellUpdate> dirtyCells;

    std::vector<std::vector<int>> bufferA; // Holds current and next frame of game
    std::vector<std::vector<int>> bufferB;
    std::vector<std::vector<int>> *currentFrame;
    std::vector<std::vector<int>> *nextFrame;

    // functions
    float calcCellNeighbors(int x, int y, Neighborhood *neigh);
    CellEvent updateCell(CellEvent cellState);
    CellEvent updateCellType(CellEvent cellState, CellEvent frameState);

public:
    Game(Rectangle gameScreen, int cellNumWidth, int cellNumHeight, int cellSize, Neighborhood *neigh);
    ~Game();

    void updateGame();

    void resetEmpty();
    void resetFull();
    void resetRandom();

    bool cellIsAlive(int row, int col);

    void flipCellState(int row, int col);

    void clickSimCell(Vector2 mousePosition);
    void simClickRadiusUp(bool increment);

    // Getters
    int getCellSize();
    int getCellNumWidth();
    int getCellNumHeight();
    int getCellState(int row, int col);
    std::vector<CellUpdate> *getDirtyCells();
    Rectangle getGameScreen();
    Rules *getRulesPtr();

    // Setters
    void setCell(int row, int col, CellEvent state);
};