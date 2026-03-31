#include "sounds.h"

Sounds::Sounds() {
	
	// Sounds
	basicPaddleHit = LoadSound("assets/basicPaddleHit.wav");
	basicWallHit = LoadSound("assets/basicWallHit.wav");
	goalScored = LoadSound("assets/goalScored.wav");
	gameOver = LoadSound("assets/gameOver.wav");
	playerWin = LoadSound("assets/playerWin.wav");
	menuClick = LoadSound("assets/menuClick.wav");

	// Music
	backgroundMusic = LoadMusicStream("assets/gameThemeTwo.wav");
}

Sounds::~Sounds() {

	UnloadSound(basicPaddleHit);
	UnloadSound(basicWallHit);
	UnloadSound(goalScored);
	UnloadSound(gameOver);
	UnloadSound(playerWin);
	UnloadSound(menuClick);

	UnloadMusicStream(backgroundMusic);
}

Music Sounds::getMusic() { return backgroundMusic; }

void Sounds::playBasicPaddleHit() { PlaySound(basicPaddleHit); }

void Sounds::playBasicWallHit() { PlaySound(basicWallHit); }

void Sounds::playGoalScored() { PlaySound(goalScored); }

void Sounds::playGameOver() { PlaySound(gameOver); }

void Sounds::playPlayerWin() {PlaySound(playerWin); }

void Sounds::playMenuClick() { PlaySound(menuClick); }
