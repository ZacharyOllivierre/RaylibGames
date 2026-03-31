#pragma once
#include "game.h"

class Graphics {

private:
	
	const Vector2 origin = {0, 0};

	int screenWidth;
	int screenHeight;
	
	int gameWidth;
	int gameHeight;

	float gameScreenScale;

	Rectangle gameDestination;
		
	int menuFontSize;
	int fontSize;
	
	RenderTexture2D gameTexture;
	RenderTexture2D gamePass1;
		
	Shader fisheye;
	Shader bloom;
	
	// private functions
	Rectangle calculateGameRec();
	void buildGameTexture(Game::GameValues snap);
	void buildStartMenu();
	void buildGameOver();
	void buildPlayerWon();

	void buildBorder(float borderThickness);
	
public:

	Graphics(int screenWidth, int screenHeight, int gameWidth, int gameHeight);
	~Graphics();
	
	void drawGame(int state, Game& game);
};
