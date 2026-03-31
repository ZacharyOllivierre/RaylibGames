#include "paddle.h"
#include "ball.h"
#include <cmath>

Paddle::Paddle(int goalSize, int width, int height, bool player, int screenWidth, int  screenHeight) {
	
	// Save screen dimensions
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	// Set default dimensions
	this->width = width;
	this->height = height;
	
	// Set player - True for human
	this->player = player;
	
	// Set goalsize - edge of screen to start of paddle
	this->goalSize = goalSize;
	
	// Reset Paddle
	this->resetPaddle();
	
	velocity = 0;

	// TO FIX
	wallBounceCoefficient = 0.30;
	friction = 0.96;
	maxSpeed = (14 * screenHeight) / 700;
}

void Paddle::updatePaddle(int move) {
	
	// move down
	if (move == 1) {
	
		velocity += 1;
	}
	// move up
	else if (move == -1) {

		velocity += -1;
	}
	// no move
	else {
		velocity *= friction; 
	}	

	// Check max speed
	if (velocity > maxSpeed) {

		velocity = maxSpeed;
	}
	else if (velocity < -maxSpeed) {

		velocity = -maxSpeed;
	}
	
	// Clamp
	if (yPosition < 0) {

		yPosition = 0;
		velocity *= -wallBounceCoefficient;
	}	
	else if (yPosition + height > screenHeight) {

		yPosition = screenHeight - height;
		velocity *= -wallBounceCoefficient;
	}

	yPosition += velocity;
}

void Paddle::resetPaddle() {
	
	// If bot - left
	if (!player) {
	
		//xPosition = goalSize;
		xPosition = screenWidth * .10;
		yPosition = (screenHeight - height) / 2; 
	}
	// If player - right
	else {
		//xPosition = screenWidth - goalSize;
		xPosition = screenWidth * .90 - width;
		yPosition = (screenHeight - height) / 2;
	}
}

// Setters
void Paddle::setXPosition(int xPosition) {this->xPosition = xPosition;}
void Paddle::setYPosition(int yPosition) {this->yPosition = yPosition;}

// Getters
bool Paddle::getPlayer() {return player;}
int Paddle::getXPosition() {return xPosition;}
int Paddle::getYPosition() {return yPosition;}
int Paddle::getBoxWidth() {return width;}
int Paddle::getBoxHeight() {return height;}
int Paddle::getVelocity() {return velocity;}
