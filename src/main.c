#include "main.h"

int main(void) {
	atexit(report_mem_leak);

	// Setup Part
	//----------------------------------------

	Game game = CreateGame();
	game.editor.level.player = &game.player;
	// EditLevel(&game.editor, "ressources/levels/Testings/thirdSave.dat");
	CreateLevel(&game.editor, 1024, 1024);
	// CreateLevel(&game.editor, 8192, 8192);

	//----------------------------------------
	// End Setup Part

	// Variable Part
	//----------------------------------------

	//----------------------------------------
	// End Variable Part

	while (!WindowShouldClose()) {
		// Update Part
		//----------------------------------------
		frameCount++;

		UpdateGame(&game);

		if (IsKeyPressed(KEY_SPACE)) SaveLevelAs(&game.editor, "ressources/levels/Testings/thirdSave.dat");

		//----------------------------------------
		// End Update Part

		BeginDrawing();
		ClearBackground(DARKGRAY);

		// World Drawing Part
		//----------------------------------------
		BeginMode2D(gameCamera);

		DisplayGameWorld(&game);
		
		EndMode2D();
		//----------------------------------------
		// End World Drawing Part

		// HUD Drawing Part
		//----------------------------------------

		DisplayGameUI(&game);

		DrawFPS(0, 100);

		//----------------------------------------
		// End HUD Drawing Part

		EndDrawing();
	}

	// Cleanup Part
	//----------------------------------------
	FreeGame(&game);
	//----------------------------------------
	// End Cleanup Part

	printf("Program closed successfully !\n");
	return 0;
}
