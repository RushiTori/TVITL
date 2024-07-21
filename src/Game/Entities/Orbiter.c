#include "Orbiter.h"
#include "Assets.h"

Orbiter CreateOrbiter(Vector2 pos, float offset, float baseAngle, float angleSpeed) {
	Orbiter orbiter = {0};
	orbiter.pos = pos;
	orbiter.offset = offset * WORLD_SCALE;
	orbiter.angle = (baseAngle / (PI * 2)) * UINT16_MAX;  // WIP
	orbiter.angleSpeed = angleSpeed;
	return orbiter;
}

void DisplayOrbiter(const Orbiter* orbiter) {
	// WIP
	static const Rectangle orbiterSource = (Rectangle){
		.x = 32,
		.y = 64,
		.width = 32,
		.height = 32,
	};
	Vector2 posOffset = (Vector2){.x = COS_TABLE[orbiter->angle], .y = SIN_TABLE[orbiter->angle]};
	posOffset = Vector2Scale(posOffset, orbiter->offset);
	Vector2 drawPos = Vector2Add(orbiter->pos, posOffset);
	Rectangle orbiterBody = (Rectangle){
		.x = drawPos.x - ORBITER_RADIUS,
		.y = drawPos.y - ORBITER_RADIUS,
		.width = ORBITER_RADIUS * 2,
		.height = ORBITER_RADIUS * 2,
	};

	DrawTexturePro(EntitySheet, orbiterSource, orbiterBody, (Vector2){0}, 0, WHITE);
}

void UpdateOrbiter(Orbiter* orbiter) { orbiter->angle += orbiter->angleSpeed * DELTA_TIME * UINT16_MAX / 2; }
