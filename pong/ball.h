#pragma once
#include "paddle.h"
#include <raylib.h>
#include <cstdlib>
#include <string>

class Ball {

private:

	int screenWidth;
	int screenHeight; 
	
	int goalWidth;	
	
	int xPosition;
	int yPosition;

	int xVelocity;
	int yVelocity;
	
	int maxVelocity;
	
	float paddleBounceCoefficient;
	float wallBounceCoefficient;
	
	int radius;
	Color ballColor;
	
	// Private function
	Color updateColor();
	void enforceMaxSpeed();
			
public:
	Ball(int sWidth, int sHeight, int xVelocity, int yVelocity, int radius, int goalWidth);
	
	void updatePosition();
	bool wallCollision();
	
	bool paddleCollision(Paddle* paddle);
	
 	bool ballIsScoredPlayer();
	bool ballIsScoredBot();

	void resetBall();

	// Setters
	void setXPosition(int xPosition);
	void setYPosition(int yPosition);
	void setXVelocity(int xVelocity);
	void setYVelocity(int yVelocity);
	void setRadius(int radius);

	// Getters
	int getXPosition();
	int getYPosition();
	int getRadius();
	Color getColor();
};
