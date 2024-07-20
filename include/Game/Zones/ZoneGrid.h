#ifndef ZONE_GRID_H
#define ZONE_GRID_H

#include "Assets.h"
#include "ByteBuffer.h"
#include "DynamicArray.h"
#include "GameCamera.h"
#include "Zone.h"
#include "ZoneGroup.h"
#include "ZoneType.h"

typedef struct ZoneGrid {
	uint16_t width;
	uint16_t height;
	Zone* zones;
	DynamicArray(ZoneGroup) groups;
} ZoneGrid;

ZoneGrid CreateZoneGridEmpty(uint32_t width, uint32_t height);
ZoneGrid CreateZoneGridFromFileData(ByteBuffer* buffer);
void FreeZoneGrid(ZoneGrid* grid);

void SerializeZoneGrid(const ZoneGrid* grid, ByteBuffer* buffer);

ZoneType GetZoneType(const ZoneGrid* grid, uint32_t x, uint32_t y);

void SetZoneType(const ZoneGrid* grid, uint32_t x, uint32_t y, ZoneType type);
void SetPlayerCollidedZone(ZoneGrid* grid, uint32_t x, uint32_t y);

void DisplayZoneGrid(const ZoneGrid* grid);
void UpdateZoneGrid(ZoneGrid* grid);

#endif	// ZONE_GRID_H
