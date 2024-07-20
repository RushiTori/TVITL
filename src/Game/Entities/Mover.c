#include "Mover.h"

Mover CreateMover(Vector2 basePos) {
	Mover mover = {0};
	mover.pos = basePos;
	return mover;
}

void SetMoverPath(Mover* mover, EntityPath* path) {
	mover->pathRef = path;
	SetEntityPathEntityRef(path, &mover->pos);
}

void DisplayMover(const Mover* mover) {
	// WIP
	DrawRectangleRec(
		(Rectangle){
			.x = mover->pos.x - (MOVER_SIZE / 2),
			.y = mover->pos.y - (MOVER_SIZE / 2),
			.width = MOVER_SIZE,
			.height = MOVER_SIZE,
		},
		BLUE);
}

void UpdateMover(Mover* mover) { UpdateEntityPath(mover->pathRef); }
