#pragma once
#include <raylib.h>
#include "types.h"
#include "graphics.h"
#include "controls.h"
#include "neighborhood.h"
#include "button.h"

const int screenWidth = 1408;
const int screenHeight = 768;

const int gameWidth = 832; // roughly 60% 16 x 52
const int gameHeight = screenHeight;
const int gameXPos = screenWidth - gameWidth; // 576 16 x 36
const int gameYPos = 0;

Rectangle gameScreen = {gameXPos, gameYPos, gameWidth, gameHeight};

const int cellSize = 4;

const int cellNumWidth = gameWidth / cellSize;
const int cellNumHeight = gameHeight / cellSize;

const int numNeighborhoods = 2;
const int neighborhoodSize = 11;
const int neighborhoodScreenSize = 176; // 16 * 11

// Array of neighborhood's dimensions and location
Rectangle neighScreenArr[numNeighborhoods];
float neighborhoodCellSize;

float buttonWidth = 96;
float buttonHeight = 96;

Rectangle rulesRect;
Rectangle rulesSectionRecs[RULE_SECTION_COUNT];

const int fps = 60;
int simulatedFrameRate = 10;
int currentFrameNum = 0;

GameState state = GameState::HOMESCREEN;

std::vector<Button> buttonVector;