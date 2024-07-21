#include "Mover.h"

#include "Assets.h"

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
	static const Rectangle moverSource = (Rectangle){
		.x = 0,
		.y = 64,
		.width = 32,
		.height = 32,
	};

	const Rectangle moverBody = (Rectangle){
		.x = mover->pos.x - (MOVER_SIZE / 2),
		.y = mover->pos.y - (MOVER_SIZE / 2),
		.width = MOVER_SIZE,
		.height = MOVER_SIZE,
	};
	
	DrawTexturePro(EntitySheet, moverSource, moverBody, (Vector2){0}, 0, WHITE);
}

void UpdateMover(Mover* mover) { UpdateEntityPath(mover->pathRef); }
