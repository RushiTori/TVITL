#include "Level.h"

Level CreateLevelFromFile(const char* filePath, Player* playerRef) {
	Level level = {0};
	level.player = playerRef;

	ByteBuffer levelData = CreateByteBufferFromFile(filePath);
	level.grid = CreateZoneGridFromFileData(&levelData);

	FreeByteBuffer(&levelData);
	return level;
}

void FreeLevel(Level* level) {
	FreeZoneGrid(&level->grid);
	FreeDynamicArray(&level->movers);
	FreeDynamicArray(&level->orbiters);
	MapDynamicArray(&level->movorbiters, FreeMovorbiter);
	FreeDynamicArray(&level->movorbiters);
	FreeDynamicArray(&level->shooters);
	FreeDynamicArray(&level->bullets);
	Player* pRef = level->player;
	*level = (Level){0};
	level->player = pRef;
}

void DisplayLevel(const Level* level) {
	DisplayZoneGrid(&level->grid);
	DisplayPlayer(level->player);

	MapDynamicArray(&level->movers, DisplayMover);
	MapDynamicArray(&level->orbiters, DisplayOrbiter);
	MapDynamicArray(&level->movorbiters, DisplayMovorbiter);
	MapDynamicArray(&level->shooters, DisplayShooter);
	MapDynamicArray(&level->bullets, DisplayBullet);
}

void UpdateLevel(Level* level) {
	MapDynamicArray(&level->movers, UpdateMover);
	MapDynamicArray(&level->orbiters, UpdateOrbiter);
	MapDynamicArray(&level->movorbiters, UpdateMovorbiter);
	for (uint32_t i = 0; i < (&level->shooters)->size; i++) {
		UpdateShooter((&level->shooters)->data + i, level);
	};
	MapDynamicArray(&level->bullets, UpdateBullet);
	UpdatePlayerInputs(level->player);

	UpdatePlayerStates(level->player);
	CollidePlayerToZoneGrid(level->player, &level->grid, false);

	UpdatePlayerStates(level->player);
	CollidePlayerToZoneGrid(level->player, &level->grid, true);

	UpdateZoneGrid(&level->grid);
}
