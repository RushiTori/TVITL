#include "Movorbiter.h"

#include "DynamicArray.h"  // I wish I could remove this but, once again, _Generic is cringe

Movorbiter CreateMovorbiter(Vector2 pos, float offset, float baseAngle, float angleSpeed, uint32_t orbiterCount) {
	Movorbiter movorbiter = {0};
	movorbiter.mover = CreateMover(pos);
	movorbiter.useMover = true;

	movorbiter.orbiters = CreateDynamicArray(Orbiter);
	ReserveDynamicArray(&movorbiter.orbiters, orbiterCount);

	float angleOff = (PI * 2) / orbiterCount;
	for (uint32_t i = 0; i < orbiterCount; i++) {
		Orbiter tmp = CreateOrbiter(pos, offset, baseAngle + angleOff * i, angleSpeed);
		PushBackDynamicArray(&movorbiter.orbiters, tmp);
	}

	ShrinkDynamicArray(&movorbiter.orbiters);

	return movorbiter;
}

void FreeMovorbiter(Movorbiter* movorbiter) { FreeDynamicArray(&movorbiter->orbiters); }

void SetMovorbiterPath(Movorbiter* movorbiter, EntityPath* path) { SetMoverPath(&movorbiter->mover, path); }

void DisplayMovorbiter(const Movorbiter* movorbiter) {
	// WIP
	if (movorbiter->useMover) DisplayMover(&movorbiter->mover);

	for (uint32_t i = 0; i < movorbiter->orbiters.size; i++) {
		DisplayOrbiter(movorbiter->orbiters.data + i);
	}
}

void UpdateMovorbiter(Movorbiter* movorbiter) {
	UpdateMover(&movorbiter->mover);

	for (uint32_t i = 0; i < movorbiter->orbiters.size; i++) {
		UpdateOrbiter(movorbiter->orbiters.data + i);
		movorbiter->orbiters.data[i].pos = movorbiter->mover.pos;
	}
}
