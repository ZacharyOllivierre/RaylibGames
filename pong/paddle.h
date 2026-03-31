#pragma once

class Paddle {

private:
	int screenWidth;
	int screenHeight;
	int goalSize;

	int xPosition;
	int yPosition;
	int width;
	int height;
	
	bool player;

	// TO FIX	
	float wallBounceCoefficient;
	int velocity;
	float friction;
	int maxSpeed;

public:

	Paddle(int goalSize, int width, int height, bool player, int screenWidth, int screenHeight);

	void updatePaddle(int move);	
	void resetPaddle();

	// Setters
	void setXPosition(int xPosition);
	void setYPosition(int yPosition);

	// Getters
	bool getPlayer();
	int getXPosition();
	int getYPosition();
	int getBoxWidth();
	int getBoxHeight();
	int getVelocity();
};
