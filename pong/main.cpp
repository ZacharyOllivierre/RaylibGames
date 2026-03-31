#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include "game.h"
#include "graphics.h"

enum GameState {

	STARTMENU,
	PLAYING,
	PLAYERWON,
	GAMEOVER
};

void checkWon(Game& game, GameState& state);

int main() {
	
	// Basics
	const int fps = 60;
	int screenWidth = GetMonitorWidth(0);
	int screenHeight = GetMonitorHeight(0);

	const int gameWidth = 1400;
	const int gameHeight = 750;
	
	// Game values
	int pointsToWin = 7;
	int ballSize = 9;
	int paddleHeight = gameHeight * .15;
	int paddleWidth = 5;
	
	// Initiallize window / audio
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
	
	InitWindow(screenWidth, screenHeight, "PONG");
	ToggleFullscreen();

	screenWidth = GetMonitorWidth(0);
	screenHeight = GetMonitorHeight(0);

	InitAudioDevice();
	SetTargetFPS(fps);

	// Seed random
	srand(time(nullptr));

	GameState state = STARTMENU;
	Game game(gameWidth, gameHeight, pointsToWin, ballSize, paddleWidth, paddleHeight);
	
	// Graphics
	Graphics gameGraphics(screenWidth, screenHeight, gameWidth, gameHeight);

	// Music
	Sounds sound;	
	PlayMusicStream(sound.getMusic());
	
	// Game loop
	while (!WindowShouldClose()) {
		
		// Update Music
		UpdateMusicStream(sound.getMusic());
		
		// Start drawing
		BeginDrawing();
		ClearBackground(BLACK);

		// Switch for each GameState
		switch (state) {

		case GameState::STARTMENU: {
			
			gameGraphics.drawGame(state, game);
	
			// Get start input
			if (IsKeyPressed(KEY_SPACE)) {
				
				sound.playMenuClick();
				state = GameState::PLAYING;
			}	
			break;
		}
		case GameState::PLAYING: {
	
			// Get input
			game.updatePlayerPaddle();
			game.updateBotPaddle();
			
			// Update state			
			game.updateBallState();	
	
			// Drawing
			gameGraphics.drawGame(state, game);

			// Check if won
			checkWon(game, state);

			break;
		}
		case GameState::GAMEOVER: {
			
			gameGraphics.drawGame(state, game);

			// Get continue input
			if (IsKeyPressed(KEY_SPACE)) {

				sound.playMenuClick();
				state = GameState::PLAYING;
			}
			break;
		}
		case GameState::PLAYERWON: {
					
			gameGraphics.drawGame(state, game);

			// Get continue input
			if (IsKeyPressed(KEY_SPACE)) {
	
				sound.playMenuClick();
				state = GameState::PLAYING;
			}
			break;
		}
		}

		// End drawing
		EndDrawing();
	}
	

	// Close
	CloseAudioDevice();
	CloseWindow();

	return 0;
}

void checkWon(Game& game, GameState& state) {

	int won = game.hasWon();
	
	// Player won
	if (won == 1) {
		game.resetGame();
		state = GameState::PLAYERWON;
	}
	else if (won == 2) {
		game.resetGame();
		state = GameState::GAMEOVER;
	}

}
