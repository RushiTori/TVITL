#ifndef GAME_H
#define GAME_H

#include "Assets.h"
#include "Common.h"
#include "Editor.h"
#include "GameCamera.h"
#include "Level.h"
#include "Mover.h"
#include "Movorbiter.h"
#include "Orbiter.h"
#include "Player.h"
#include "ZoneGrid.h"

typedef enum GameState {
	Playing = 0,
	Editing,
} GameState;

typedef struct Game {
	Player player;
	Level currentLevel;
	GameState state;
	Editor editor;
} Game;

Game CreateGame();
void FreeGame(Game* game);

void LoadLevel(Game* game, const char* levelFilePath);

void DisplayGameWorld(const Game* game);
void DisplayGameUI(const Game* game);
void UpdateGame(Game* game);

#endif	// GAME_H
