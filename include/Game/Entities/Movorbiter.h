#ifndef MOVORBITER_H
#define MOVORBITER_H

#include "Common.h"
#include "DynamicArrayTypes.h"
#include "Mover.h"
#include "Orbiter.h"

typedef struct Movorbiter {
	Mover mover;
	bool useMover;
	OrbiterArray orbiters;	// I wish I could use DynamicArray(Orbiter) but _Generic doesn't allow incomplete types
} Movorbiter;

Movorbiter CreateMovorbiter(Vector2 pos, float offset, float baseAngle, float angleSpeed, uint32_t orbiterCount);
void FreeMovorbiter(Movorbiter* movorbiter);

void SetMovorbiterPath(Movorbiter* movorbiter, EntityPath* path);

void DisplayMovorbiter(const Movorbiter* movorbiter);
void UpdateMovorbiter(Movorbiter* movorbiter);

#endif	// MOVORBITER_H
