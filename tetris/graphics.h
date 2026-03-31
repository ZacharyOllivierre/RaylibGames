#pragma once
#include "raylib.h"
#include "board.h"

class Graphics
{
public:
    Graphics(Board *board);

    void printBoard();

private:
    Rectangle screenRec;
    Rectangle boardRec;

    Board *board;

    int tileSize;

    void centerRecInRec(Rectangle &recOne, Rectangle &recTwo);
    void initBoardRec();
    void initScreenRec();
    void initTiles();
};