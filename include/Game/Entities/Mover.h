#ifndef MOVER_H
#define MOVER_H

#include "Common.h"
#include "EntityPath.h"

#define MOVER_SIZE WORLD_SCALE

typedef struct Mover {
	Vector2 pos;
	EntityPath* pathRef;
} Mover;

Mover CreateMover(Vector2 basePos);

void SetMoverPath(Mover* mover, EntityPath* path);

void DisplayMover(const Mover* mover);
void UpdateMover(Mover* mover);

#endif	// MOVER_H
