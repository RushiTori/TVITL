#ifndef LEVEL_H
#define LEVEL_H

#include "Bullet.h"
#include "ByteBuffer.h"
#include "DynamicArray.h"
#include "Mover.h"
#include "Movorbiter.h"
#include "Orbiter.h"
#include "Player.h"
#include "Shooter.h"
#include "ZoneGrid.h"

typedef struct Level {
	Player* player;
	ZoneGrid grid;
	DynamicArray(Mover) movers;
	DynamicArray(Orbiter) orbiters;
	DynamicArray(Movorbiter) movorbiters;
	DynamicArray(Shooter) shooters;
	DynamicArray(Bullet) bullets;
} Level;

Level CreateLevelFromFile(const char* filePath, Player* playerRef);
void FreeLevel(Level* level);

void DisplayLevel(const Level* level);
void UpdateLevel(Level* level);

#endif	// LEVEL_H
