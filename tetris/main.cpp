#include "raylib.h"
#include <string>

#include "graphics.h"
#include "board.h"

using std::string;

const int WIDTH = 900;
const int HEIGHT = 800;

int main()
{
    InitWindow(WIDTH, HEIGHT, "Tetris");

    Board board;
    Graphics graphics(&board);

    while (!WindowShouldClose())
    {
        // game loop
        BeginDrawing();

        ClearBackground(BLUE);

        graphics.printBoard();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}