#pragma once
#include <raylib.h>

class Sounds {

private:
	Sound basicPaddleHit;	
	Sound basicWallHit;
	Sound goalScored;
	Sound gameOver;
	Sound playerWin;
	Sound menuClick;

	Music backgroundMusic;

public:

	Sounds();
	~Sounds();
	
	Music getMusic(); 
	void playBasicPaddleHit();
	void playBasicWallHit();
	void playGoalScored();
	void playGameOver();
	void playPlayerWin();
	void playMenuClick();	
};
