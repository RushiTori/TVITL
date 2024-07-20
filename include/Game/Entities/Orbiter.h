#ifndef ORBITER_H
#define ORBITER_H

#include "Common.h"
#include "EntityPath.h"

#define ORBITER_RADIUS (WORLD_SCALE / 2)

typedef struct Orbiter {
	Vector2 pos;
	float offset;
	uint16_t angle;
	float angleSpeed;
} Orbiter;

Orbiter CreateOrbiter(Vector2 pos, float offset, float baseAngle, float angleSpeed);

void DisplayOrbiter(const Orbiter* orbiter);
void UpdateOrbiter(Orbiter* orbiter);

#endif	// ORBITER_H
