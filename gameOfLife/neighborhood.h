#pragma once
#include <vector>
#include <raylib.h>
#include "util.h"

class Neighborhood
{

private:
    int neighCellSize; // cell pixel dimentions
    int neighSize;     // 3 for 3b3 grid

    int neighClickRadius; // Radius of click

    std::vector<std::vector<int>> neighArray;

    Rectangle neighScreen;

    // Holds locations of active neighborhood cells
    std::vector<std::pair<int, int>> activeOffsets;

    bool neighChanged;

    // functions
    void flipNeighCell(int row, int col);

public:
    Neighborhood(int neighSize, int neighCellSize, Rectangle neighScreen);
    ~Neighborhood();

    void resetFull();
    void resetEmpty();
    void resetRandom();

    void updateNeighborOffsets();

    void clickNeighCell(Vector2 mousePosition); // Repeated code of game click - move into utility
    void neighClickRadiusUp(bool increment);

    // getters
    int getNeighArrayRowCol(int row, int col);
    Rectangle getNeighScreen();
    int getNeighSize();
    int getNeighCellSize();
    bool getNeighChanged();
    std::vector<std::pair<int, int>> *getNeighOffsets();

    // setters
    void setNeighCell(int row, int col, int state);
    void setNeighChanged(bool changed);
};