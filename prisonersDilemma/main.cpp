#include "raylib.h"

#include "game.h"
#include "players/playerList.h"
#include "gameViewer.h"

const Vector2 screenDimensions = {1400, 800};

int main()
{
    InitWindow(screenDimensions.x, screenDimensions.y, "Prisoners Dilemma");
    InitAudioDevice();
    SetTargetFPS(60);

    Random r;
    TitForTat tft;
    AlwaysCooperate ac;
    Spitefull s;

    Game game(&r, &s, 5);
    game.playGame();
    // game.gameHistoryDebugPrint();

    GameViewer viewer(game);

    Sound stepSound = LoadSound("bop.wav");

    bool autoRun;
    float autoSecondsPerStep = 1;
    float autoTimerSeconds = 0;

    while (!WindowShouldClose())
    {
        if (autoRun)
        {
            autoTimerSeconds += GetFrameTime();
            if (autoTimerSeconds >= autoSecondsPerStep)
            {
                if (viewer.isFinished())
                    autoRun = false;
                else
                {
                    viewer.step();
                    autoTimerSeconds = 0;
                }
            }
        }

        // Controls
        if (IsKeyPressed(KEY_SPACE))
        {
            viewer.step();
            PlaySound(stepSound);
        }
        if (IsKeyPressed(KEY_A))
        {
            autoRun = true;
        }
        if (IsKeyPressed(KEY_UP))
        {
            autoSecondsPerStep -= 0.3;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            autoSecondsPerStep += 0.3;
        }

        BeginDrawing();

        ClearBackground(Color{25, 30, 40, 255});
        viewer.draw();
        EndDrawing();
    }

    return 0;
}
