#include "neighborhood.h"

Neighborhood::Neighborhood(int neighSize, int neighCellSize, Rectangle neighScreen)
{
    this->neighScreen = neighScreen;
    this->neighCellSize = neighCellSize;
    this->neighSize = neighSize;

    // set all to 0
    neighArray = std::vector<std::vector<int>>(neighSize, std::vector<int>(neighSize, 0));

    neighChanged = true; // updated by control class pulled from graphics to conditionally update render

    neighClickRadius = 1;

    resetEmpty();
}

Neighborhood::~Neighborhood() {}

void Neighborhood::resetFull()
{
    for (int y = 0; y < neighSize; y++)
    {

        for (int x = 0; x < neighSize; x++)
        {
            neighArray[y][x] = 1;
        }
    }

    updateNeighborOffsets();
}

void Neighborhood::resetEmpty()
{
    for (int y = 0; y < neighSize; y++)
    {

        for (int x = 0; x < neighSize; x++)
        {
            neighArray[y][x] = 0;
        }
    }

    updateNeighborOffsets();
}

void Neighborhood::resetRandom()
{
    for (int y = 0; y < neighSize; y++)
    {

        for (int x = 0; x < neighSize; x++)
        {
            // 20% chance on
            if (GetRandomValue(1, 5) == 1)
            {
                neighArray[y][x] = 1;
            }
        }
    }

    updateNeighborOffsets();
}

// this function uses the neighborhood mask neighArray to calculate offsets of active cells from center
void Neighborhood::updateNeighborOffsets()
{
    activeOffsets.clear();

    for (int row = 0; row < neighSize; row++)
    {
        for (int col = 0; col < neighSize; col++)
        {
            if (neighArray[row][col] == 1)
            {
                activeOffsets.push_back({row - neighSize / 2, col - neighSize / 2});
            }
        }
    }

    neighChanged = true;
}

// Flips cell state of all cells within circular simClickRadius
void Neighborhood::clickNeighCell(Vector2 mousePosition)
{
    int row = (mousePosition.y - neighScreen.y) / neighCellSize;
    int col = (mousePosition.x - neighScreen.x) / neighCellSize;

    // for every cell in box of size radius - contains circle
    for (int y = row - neighClickRadius; y < row + neighClickRadius; y++)
    {

        for (int x = col - neighClickRadius; x < col + neighClickRadius; x++)
        {
            // If cell to consider is within game grid
            if (y >= 0 && y < neighSize && x >= 0 && x < neighSize)
            {

                int distFromCenterY = y - row;
                int distFromCenterX = x - col;

                // If cell is within circle to draw distX^2 + distY^2 <= radius^2
                if (squareInt(distFromCenterX) + squareInt(distFromCenterY) < squareInt(neighClickRadius))
                {
                    flipNeighCell(y, x);
                }
            }
        }
    }

    updateNeighborOffsets();
}

// Increments or Decrements click radius based on bool
void Neighborhood::neighClickRadiusUp(bool increment)
{
    int sizeStep = 1;

    // If size up or down
    increment ? neighClickRadius += sizeStep : neighClickRadius -= sizeStep;

    // Bind radius to more than 0 less than larger game size
    if (neighClickRadius < 1)
    {
        neighClickRadius = 1;
    }
    else if (neighClickRadius > neighSize / 2 + 1)
    {
        neighClickRadius = neighSize / 2 + 1;
    }
}

void Neighborhood::flipNeighCell(int row, int col)
{
    neighArray[row][col] ? neighArray[row][col] = 0 : neighArray[row][col] = 1;
}

// getters
int Neighborhood::getNeighArrayRowCol(int row, int col) { return neighArray[row][col]; }
Rectangle Neighborhood::getNeighScreen() { return neighScreen; }
int Neighborhood::getNeighSize() { return neighSize; }
int Neighborhood::getNeighCellSize() { return neighCellSize; }
bool Neighborhood::getNeighChanged() { return neighChanged; }
std::vector<std::pair<int, int>> *Neighborhood::getNeighOffsets() { return &activeOffsets; }

// setters
void Neighborhood::setNeighCell(int row, int col, int state)
{
    neighArray[row][col] = state;
    updateNeighborOffsets();
}
void Neighborhood::setNeighChanged(bool changed) { neighChanged = changed; }