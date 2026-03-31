#include "graphics.h"

Graphics::Graphics(Board *board)
{
    this->board = board;

    initScreenRec();

    initBoardRec();
}

void Graphics::printBoard()
{
    Color boardBackground = RED;

    // Draw board
    DrawRectangleRec(boardRec, boardBackground);

    // Draw Squares
    for (int y = 0; y < Board::HEIGHT; y++)
    {
        for (int x = 0; x < Board::WIDTH; x++)
        {
            DrawRectangleRec();
        }
    }
}

// Updates rec twos x and y positions to be center in rec one
void Graphics::centerRecInRec(Rectangle &recOne, Rectangle &recTwo)
{
    recTwo.x = (recOne.width / 2 + recOne.x) - recTwo.width / 2;
    recTwo.y = (recOne.height / 2 + recOne.y) - recTwo.height / 2;
}

// Set board rectangle to center of screen, set size to percent of height and proportional width
// Inits tiles - spagetti function
void Graphics::initBoardRec()
{
    float percentHeight = 0.8;

    boardRec.height = screenRec.height * percentHeight;

    initTiles();

    boardRec.width = tileSize * Board::WIDTH;

    centerRecInRec(screenRec, boardRec);
}

void Graphics::initScreenRec()
{
    screenRec.height = GetScreenHeight();
    screenRec.width = GetScreenWidth();
    screenRec.x = 0;
    screenRec.y = 0;
}

void Graphics::initTiles()
{
    tileSize = boardRec.height / Board::HEIGHT;
}