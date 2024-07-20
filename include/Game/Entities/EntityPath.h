#ifndef ENTITY_PATH_H
#define ENTITY_PATH_H

#include "Common.h"
#include "DynamicArrayTypes.h"

typedef struct EntityPath {
	Vector2* entityPos;
	float speed;
	Vector2Array anchors;  // I wish I could use DynamicArray(Vector2) but _Generic doesn't allow incomplete types
	uint32_t currAnchorIdx;
	bool loops;
} EntityPath;

EntityPath CreateEntityPath(Vector2* anchors, uint32_t anchorsLen, float speed);
void FreeEntityPath(EntityPath* path);

void SetEntityPathEntityRef(EntityPath* restrict path, Vector2* restrict posRef);

void DisplayEntityPath(const EntityPath* path);
void UpdateEntityPath(EntityPath* path);

#endif	// ENTITY_PATH_H
