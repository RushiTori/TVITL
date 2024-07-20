#include "EntityPath.h"

#include "DynamicArray.h"  // I wish I could remove this but, once again, _Generic is cringe

EntityPath CreateEntityPath(Vector2* anchors, uint32_t anchorsLen, float speed) {
	EntityPath path = {0};

	path.anchors = CreateDynamicArray(Vector2);
	ReserveDynamicArray(&path.anchors, anchorsLen);
	memcpy(path.anchors.data, anchors, sizeof(Vector2) * anchorsLen);

	path.anchors.size = anchorsLen;
	path.speed = speed;
	path.loops = Vector2Equals(anchors[0], anchors[anchorsLen - 1]);

	return path;
}

void FreeEntityPath(EntityPath* path) { FreeDynamicArray(&path->anchors); }

void SetEntityPathEntityRef(EntityPath* restrict path, Vector2* restrict posRef) {
	path->entityPos = posRef;
	*path->entityPos = path->anchors.data[0];
	path->speed = fabsf(path->speed);
	path->currAnchorIdx = 0;
}

uint32_t GetNextAnchorIdx(EntityPath* path) {
	uint32_t nextAnchorIdx = path->currAnchorIdx;
	if (path->speed >= 0) {
		nextAnchorIdx++;
	} else {
		nextAnchorIdx--;
	}
	nextAnchorIdx %= path->anchors.size;
	return nextAnchorIdx;
}

void UpdateEntityPath(EntityPath* path) {
	if (!path) return;
	if (!path->entityPos) return;

	uint32_t nextAnchorIdx = GetNextAnchorIdx(path);
	Vector2 dir = Vector2Subtract(path->anchors.data[nextAnchorIdx], *path->entityPos);
	float maxDist = Vector2Length(dir);
	float dist = fabsf(path->speed) * WORLD_SCALE * DELTA_TIME;

	while (dist >= maxDist) {
		*path->entityPos = path->anchors.data[nextAnchorIdx];
		path->currAnchorIdx = nextAnchorIdx;
		if (!path->loops && (nextAnchorIdx == 0 || nextAnchorIdx == path->anchors.size - 1)) path->speed *= -1;

		nextAnchorIdx = GetNextAnchorIdx(path);
		dir = Vector2Subtract(path->anchors.data[nextAnchorIdx], *path->entityPos);
		dist -= maxDist;
		maxDist = Vector2Length(dir);
	}

	dir = Vector2Normalize(dir);
	dir = Vector2Scale(dir, dist);
	*path->entityPos = Vector2Add(*path->entityPos, dir);
}

void DisplayEntityPath(const EntityPath* path) {
	for (uint32_t i = 0; i < path->anchors.size - 1; i++) {
		DrawLineEx(path->anchors.data[i], path->anchors.data[i + 1], 1, GOLD);
	}

	if (path->loops) DrawLineEx(path->anchors.data[0], path->anchors.data[path->anchors.size - 1], 1, GOLD);
}
