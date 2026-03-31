#include "board.h"

Board::Board()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            board[y][x] = 1;
        }
    }
}