#include "graphics.h"

Graphics::Graphics(int screenWidth, int screenHeight, int gameWidth, int gameHeight) {

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->gameWidth = gameWidth;
	this->gameHeight = gameHeight;
	
	// Load game / cabinet texture 
	gameTexture = LoadRenderTexture(gameWidth, gameHeight);
        SetTextureWrap(gameTexture.texture, TEXTURE_WRAP_CLAMP);
                                      
	gamePass1 = LoadRenderTexture(gameWidth, gameHeight);
	SetTextureWrap(gamePass1.texture, TEXTURE_WRAP_CLAMP);

	// Load shader
	fisheye = LoadShader(nullptr, "fisheye.fs");
	bloom = LoadShader(nullptr, "bloom.fs");	
	
	// Set font size - needs to be fixed
	menuFontSize = 80;
	fontSize = 40;	
	
	// Caclulate game dest rectangle
	gameDestination = calculateGameRec();
}

Graphics::~Graphics() {

	UnloadRenderTexture(gameTexture);
	UnloadRenderTexture(gamePass1);
	
	UnloadShader(fisheye);
	UnloadShader(bloom);
}

void Graphics::drawGame(int state, Game& game) {
	
	ClearBackground(BLACK);
	
	// lazy - need to fix
	BeginTextureMode(gameTexture);
	ClearBackground(BLACK);
	
	switch (state) {

		case 0:
			buildStartMenu();
			break;

		case 1:
			buildGameTexture(game.getGameValues());
			break;

		case 2:
			buildPlayerWon();
			break;

		case 3:
			buildGameOver();
			break;
	} 
	EndTextureMode();
 
	// Enable shader - pass1 - fisheye
	BeginTextureMode(gamePass1);
	BeginShaderMode(fisheye);

	DrawTexturePro(
   		gameTexture.texture,
    		{0, 0, (float)gameTexture.texture.width, - (float)gameTexture.texture.height},
    		{0, 0, (float)gameTexture.texture.width, (float)gameTexture.texture.height},
    		origin,                              
    		0.0f,                               	// rotation
    		WHITE					// tint
	);
	EndShaderMode();
	EndTextureMode();

	// Enable bloom - pass 2
	BeginShaderMode(bloom);
	
	DrawTexturePro(
                gamePass1.texture,                                                                  
                {0, 0, (float)gamePass1.texture.width, - (float)gamePass1.texture.height},        
                gameDestination,                                                   
                origin,      	                        // origin 
                0.0f,                                   // rotation 
                WHITE                                   // tint                                       
        );

	EndShaderMode();
}

void Graphics::buildStartMenu() {                   
	
	buildBorder(gameHeight * 0.01);
	
        DrawText("PONG", (gameWidth - MeasureText("PONG", menuFontSize)) / 2, gameHeight * .25, menuFontSize, WHITE);

        DrawText("SPACE TO CONTINUE", (gameWidth - MeasureText("SPACE TO CONTINUE", fontSize)) / 2, gameHeight * .75, fontSize, WHITE); 
}       
        
void Graphics::buildGameOver() {                   
       
	buildBorder(gameHeight * 0.01);
  
        DrawText("GAME OVER", (gameWidth - MeasureText("GAME OVER", menuFontSize)) / 2, gameHeight * .25, menuFontSize, RED);
        DrawText("SPACE TO CONTINUE", (gameWidth - MeasureText("SPACE TO CONTINUE", fontSize)) / 2, gameHeight * .75, fontSize, WHITE);
}                                                                                                        
        
void Graphics::buildPlayerWon() {

	buildBorder(gameHeight * 0.01); 

        // You win message
        DrawText("YOU WIN", (gameWidth - MeasureText("YOU WIN", fontSize)) / 2, gameHeight * .25, fontSize, BLUE);      
        DrawText("SPACE TO CONTINUE", (gameWidth - MeasureText("SPACE TO CONTINUE", fontSize)) / 2, gameHeight * .75, fontSize, WHITE);
}

Rectangle Graphics::calculateGameRec() {
	
	// Calculate gameWindow to screenSize scale
	float scaleX = static_cast<float>(screenWidth) / static_cast<float>(gameWidth);
	float scaleY = static_cast<float>(screenHeight) / static_cast<float>(gameHeight);
	
	// Set scale to smaller value 
	if (scaleX < scaleY) { gameScreenScale = scaleX; }	
	else { gameScreenScale = scaleY; }

	// Calculate game destination rectangle
	float destWidth, destHeight, destX, destY;

	destWidth = gameWidth * gameScreenScale;
	destHeight = gameHeight * gameScreenScale;
	
	destX = (screenWidth - destWidth) / 2;
	destY = (screenHeight - destHeight) / 2;

	return {destX, destY, destWidth, destHeight};
}

void Graphics::buildGameTexture(Game::GameValues snap) {

	buildBorder(gameHeight * 0.0025);

        // Draw ball 
        DrawCircle(snap.ballXPos, snap.ballYPos, snap.ballRadius, snap.ballColor);                   
        
        // Draw paddle
        DrawRectangle(snap.playerPaddleX, snap.playerPaddleY, snap.playerPaddleWidth, snap.playerPaddleHeight, WHITE);
        DrawRectangle(snap.botPaddleX, snap.botPaddleY, snap.botPaddleWidth, snap.botPaddleHeight, WHITE);

        // Draw Score 
        DrawText(std::to_string(snap.botScore).c_str(), gameWidth * .25, gameHeight * .10, fontSize, WHITE);                                                                                             
        DrawText(std::to_string(snap.playerScore).c_str(), gameWidth * .75, gameHeight * .10, fontSize, WHITE);        
}

void Graphics::buildBorder(float borderThickness) {

        Rectangle border = {0, 0, static_cast<float>(gameWidth), static_cast<float>(gameHeight)};

        DrawRectangleLinesEx(border, borderThickness, WHITE);
}
