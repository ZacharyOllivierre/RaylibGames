#pragma once
#include <raylib.h>
#include <string>
#include "ball.h"
#include "paddle.h"
#include "sounds.h"

class Game {

private:

	// Game Literals
	int goalWidth;
	int ballSize;

	int playerScore;
	int botScore;
	int pointsToWin;	

	int gameWidth;
	int gameHeight;
	
	Paddle* playerPaddle;
	Paddle* botPaddle;

	Ball* ball = nullptr;

	Sounds sound;

public:
	
	// Game creation for graphics class                                                              
        struct GameValues {                                                                              
                int ballXPos, ballYPos, ballRadius; Color ballColor;                               
                int playerPaddleX, playerPaddleY, playerPaddleWidth, playerPaddleHeight;                 
                int botPaddleX, botPaddleY, botPaddleWidth, botPaddleHeight;                             
                int playerScore, botScore;                                                               
        };

	Game(int sWidth, int sHeight, int pointsToWin, int ballSize, int paddleWidth, int paddleHeight);
	
	void resetGame();

	void updateBallState();
	void updatePlayerPaddle();
	void updateBotPaddle();
	
	int hasWon(); // Checks both player and bot 0 = none 1 = p1 win 2 = p2 win

	// Getter	
	GameValues getGameValues();
};
