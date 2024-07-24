#include "Game.h"

Game CreateGame() {
	InitSinCosTable();
	InitWindow(800, 800, "The Void In The Land (TVITL) in Raylib");
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_ALWAYS_RUN);

	LoadAssets();
	InitGameCamera();

	Game game = {0};

	game.player = CreatePlayer((Vector2){
		.x = GetScreenWidth() * 0.5f,
		.y = GetScreenHeight() * 0.5f,
	});

	game.state = Playing;

	game.editor = CreateEditor();

	return game;
}

void FreeGame(Game* game) {
	FreeLevel(&game->currentLevel);
	FreeEditor(&game->editor);

	UnloadAssets();
	CloseWindow();
}

void LoadLevel(Game* game, const char* levelFilePath) {
	FreeLevel(&game->currentLevel);
	game->currentLevel = CreateLevelFromFile(levelFilePath, &game->player);
	SetCameraPosWorld(Vector2Add(game->player.pos, Vector2Scale(game->player.sizes, 0.5f)));
	SetCameraZoom(1);
}

void DisplayGameWorld(const Game* game) {
	if (game->state == Playing) {
		DisplayLevel(&game->currentLevel);
	} else if (game->state == Editing) {
		DisplayEditorWorld(&game->editor);
	}
}

void DisplayGameUI(const Game* game) {
	if (game->state == Playing) {
		// WIP
	} else if (game->state == Editing) {
		DisplayEditorUI(&game->editor);
	}
}

void UpdateGame(Game* game) {
	// WIP
	if (game->state == Playing) {
		UpdateLevel(&game->currentLevel);
		UpdateGameCamera(Vector2AddValue(game->player.pos, 0.5), PLAYER_SPEED * 1.25);
	} else if (game->state == Editing) {
		UpdateEditor(&game->editor);
	}
}
