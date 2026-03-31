#include "ball.h"

Ball::Ball(int sWidth, int sHeight, int xVelocity, int yVelocity, int radius, int goalWidth) {
	
	// Set screen dimensions
	screenWidth = sWidth;
	screenHeight = sHeight;
	this->goalWidth = goalWidth;

	// Set ball to middle
	xPosition = screenWidth / 2;
	yPosition = screenHeight / 2;
	
	// Starting velocity - radius
	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;
	this->radius = radius;

	// Set max velocity 
	maxVelocity = 20 * screenWidth / 1300;

	// NEED TO FIX
	wallBounceCoefficient = .95;
	paddleBounceCoefficient = 1.3;

	// Reset ball
	this->resetBall();
}


void Ball::updatePosition() {
	
	// Enforce max velocity
	enforceMaxSpeed();
	
	// Update Positions	
	xPosition += xVelocity;
	yPosition += yVelocity;
}

bool Ball::wallCollision() {
	
	bool collision = false;

	// Vertical Collision
	if (yPosition <= 0) {
		
		yPosition = 0 + radius;
		yVelocity *= -1 * wallBounceCoefficient;
		collision = true;
	}
	else if (yPosition >= screenHeight) {

		yPosition = screenHeight - radius;
		yVelocity *= -1 * wallBounceCoefficient;
		collision = true;
	}
	
	// Update color
	if (collision) {ballColor = updateColor();}

	return collision;
}

bool Ball::paddleCollision(Paddle* paddle) {
	
	bool collision = false;	
	int paddleVelocity = paddle->getVelocity();
	float paddleVelocityConversion = 0.75;
	char collisionSide = 0;
	
	// Get paddle position and convert to Rectangle
	Rectangle rec {static_cast<float>(paddle->getXPosition()), 
		       static_cast<float>(paddle->getYPosition()), 
		       static_cast<float>(paddle->getBoxWidth()), 
		       static_cast<float>(paddle->getBoxHeight())};
	
	Vector2 ball = {static_cast<float>(xPosition), static_cast<float>(yPosition)};

	// If ball and paddle collision	
	if (CheckCollisionCircleRec(ball, radius, rec)) {
		collision = true;
	
		// Check overlap
		float overlapLeft = (xPosition + radius) - rec.x;
		float overlapRight = (rec.x + rec.width) - (xPosition - radius);
		float overlapTop = (yPosition + radius) - rec.y;
		float overlapBottom = (rec.y + rec.height) - (yPosition - radius);
		
		// Find smallest overlap
		float minOverlap = overlapLeft;
		collisionSide = 'L';
		
		if (overlapRight < minOverlap) {
			
			collisionSide = 'R';
			minOverlap = overlapRight;
		}
		if (overlapTop < minOverlap) { 

			collisionSide = 'T'; 
			minOverlap = overlapTop;
		}
		if (overlapBottom < minOverlap) { 

			collisionSide = 'B';
			minOverlap = overlapBottom; 
		}
		
	}

	// Bounce ball based on collision side
	switch (collisionSide) {

	case 'L':
		
		// Set ball outside paddle
		xPosition = rec.x - radius;
			
		// Flip and reduce xVelocity
		xVelocity *= -paddleBounceCoefficient;			
		
		// Add paddle velocity to ball y velocity
		yVelocity += paddleVelocity * paddleVelocityConversion; 
		break;
	
	case 'R':
		
		xPosition = rec.x + rec.width + radius;
		xVelocity *= -paddleBounceCoefficient;
		yVelocity += paddleVelocity * paddleVelocityConversion;
		break;

	case 'T':

		yPosition = rec.y - radius;
		yVelocity *= -paddleBounceCoefficient;
		
		// if paddle is going up
		if (paddleVelocity < 0 ) {
			
			yVelocity += paddleVelocity * paddleVelocityConversion;
				
		}
		break;

	case 'B':

		yPosition = rec.y + rec.height + radius;
		yVelocity *= -1 * paddleBounceCoefficient;
		
		// if paddle is going down
		if (paddleVelocity > 0) {

			yVelocity += paddleVelocity * paddleVelocityConversion;
		}
		break;
	}
	
	// Update color
	if (collision) {ballColor = updateColor();}

	// Return if collision or not (for sound)
	return collision;
}

bool Ball::ballIsScoredPlayer() {

	if (xPosition <= goalWidth) {
		
		return true;
	}
	return false;
}

bool Ball::ballIsScoredBot() {
	
	if (xPosition >= screenWidth - goalWidth) {

		return true;
	}
	return false;
}

void Ball::resetBall() {
	
	int randomNum = rand() % 4;

	// set ball position to center
	xPosition = screenWidth / 2;
	yPosition = screenHeight / 2;
	
	// set random velocity
	if (randomNum == 0) {
	
		xVelocity = rand() % 3 + 4;
		yVelocity = rand() % 3 + 4;
	}
	else if (randomNum == 1) {
	
		xVelocity = (rand() % 3 + 4) * -1;
		yVelocity = (rand() % 3 + 4) * -1;
	}
	else if (randomNum == 2) {
		
		xVelocity = (rand() % 3 + 4) * -1;
		yVelocity = (rand() % 3 + 4);
	}
	else if (randomNum == 3) {
		
		xVelocity = (rand() % 3 + 4);
		yVelocity = (rand() % 3 + 4) * -1;
	}

	// Reset color                                                                               
        ballColor = updateColor();  
}

// Private
Color Ball::updateColor() {
	
	int averageVelocity = (abs(xVelocity) + abs(yVelocity)) / 2.0;
	float percentOfMaxSpeed = averageVelocity / static_cast<float>(maxVelocity);

	// Set green and blue to percent equivalent of 255
	unsigned char green = 255 - (255 * percentOfMaxSpeed);
	unsigned char blue = 255 -(255 * percentOfMaxSpeed);
	unsigned char red = 255;
	unsigned char opaque = 255;	

	return {red, green, blue, opaque};
}

void Ball::enforceMaxSpeed() {

	// Clamp x and y velocity to max speed
	if (xVelocity > maxVelocity) { xVelocity = maxVelocity; }
	else if (xVelocity < -maxVelocity) { xVelocity = -maxVelocity; }
	
	if (yVelocity > maxVelocity) { yVelocity = maxVelocity; }
	else if (yVelocity < -maxVelocity) { yVelocity = -maxVelocity; }
}

// Setters
void Ball::setXPosition(int xPosition) {this->xPosition = xPosition;}
void Ball::setYPosition(int yPosition) {this->yPosition = yPosition;}
void Ball::setXVelocity(int xVelocity) {this->xVelocity = xVelocity;}
void Ball::setYVelocity(int yVelocity) {this->yVelocity = yVelocity;}
void Ball::setRadius(int radius) {this->radius = radius;}

// Getters
int Ball::getXPosition() {return xPosition;}
int Ball::getYPosition() {return yPosition;}
int Ball::getRadius() {return radius;}
Color Ball::getColor() {return ballColor;}
