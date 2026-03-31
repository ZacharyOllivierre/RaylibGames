#include "game.h"

Game::Game(int sWidth, int sHeight, int pointsToWin, int ballSize, int paddleWidth, int paddleHeight) {
	
	// Set literals
	gameWidth = sWidth;
	gameHeight = sHeight;
	
	// Set goalWidth to 05% of screen
	goalWidth = gameWidth * .05;
	
	this->pointsToWin = pointsToWin;
	this->ballSize = ballSize;

	// Set scores to 0
	playerScore = 0;
	botScore = 0;
	
	// Create ball object
	ball = new Ball(gameWidth, gameHeight, 0, 0, ballSize, goalWidth);

	// Create paddles
	playerPaddle = new Paddle(goalWidth, paddleWidth, paddleHeight, true, gameWidth, gameHeight);
	botPaddle = new Paddle(goalWidth, paddleWidth, paddleHeight, false, gameWidth, gameHeight);
}

void Game::resetGame() {
	
	// Reset Scores
	playerScore = 0;
	botScore = 0;

	// Reset paddles
	playerPaddle->resetPaddle();
	botPaddle->resetPaddle();
}

void Game::updateBallState() {
	
	// Update ball
	ball->updatePosition();
	if (ball->wallCollision()) { sound.playBasicWallHit(); }
	if (ball->paddleCollision(playerPaddle)) { sound.playBasicPaddleHit(); }
	if (ball->paddleCollision(botPaddle)) { sound.playBasicPaddleHit(); }
		
	// Check if scored
	if (ball->ballIsScoredPlayer()) {
			
		// Add to score and reset ball - play sound
		playerScore += 1;
		sound.playGoalScored();		
		ball->resetBall();
	}
	else if(ball->ballIsScoredBot()) {
		
		botScore += 1;
		sound.playGoalScored();
		ball->resetBall();
	}
}

void Game::updatePlayerPaddle() {
	
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
		
		playerPaddle->updatePaddle(1);	
	}
	else if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
		
		playerPaddle->updatePaddle(-1);
	}
	else {
		playerPaddle->updatePaddle(0);
	}
	
}

void Game::updateBotPaddle() {
	
	// Get ball y position
	int ballYPos = ball->getYPosition();
	int paddleCenter = botPaddle->getYPosition() + botPaddle->getBoxHeight() / 2;
	
	// Move down
	if (paddleCenter < ballYPos) {
		
		botPaddle->updatePaddle(1);
	}
	// Move up
	else if (paddleCenter > ballYPos) {

		botPaddle->updatePaddle(-1);
	}
	else {
		botPaddle->updatePaddle(0);
	}
}

int Game::hasWon() {
	
	// Player has won
	if (playerScore >= pointsToWin) {
		
		sound.playPlayerWin();
		return 1;
	}
	
	// Bot has won
	if (botScore >= pointsToWin) {
		
		sound.playGameOver();
		return 2;
	}
	
	// No one has won
	return 0;
}

Game::GameValues Game::getGameValues() {
	
	// Returns current game values
	return GameValues {
                ball->getXPosition(),
                ball->getYPosition(),
                ball->getRadius(),
                ball->getColor(),

                playerPaddle->getXPosition(),                                                   
                playerPaddle->getYPosition(),                                                   
                playerPaddle->getBoxWidth(),                                                    
                playerPaddle->getBoxHeight(),                                                   
                botPaddle->getXPosition(),                                                      
                botPaddle->getYPosition(),                                                      
                botPaddle->getBoxWidth(),                                                       
                botPaddle->getBoxHeight(),                                                      
                
                playerScore,                                                                    
                botScore
        };
}



