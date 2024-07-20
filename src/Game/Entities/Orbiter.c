#include "Orbiter.h"

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
	Vector2 posOffset = (Vector2){.x = COS_TABLE[orbiter->angle], .y = SIN_TABLE[orbiter->angle]};
	posOffset = Vector2Scale(posOffset, orbiter->offset);
	Vector2 drawPos = Vector2Add(orbiter->pos, posOffset);
	DrawCircleSector(drawPos, ORBITER_RADIUS, 0, 360, 12, BLUE);
}

void UpdateOrbiter(Orbiter* orbiter) { orbiter->angle += orbiter->angleSpeed * DELTA_TIME * UINT16_MAX / 2; }
