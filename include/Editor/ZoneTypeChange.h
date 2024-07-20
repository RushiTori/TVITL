#ifndef ZONE_TYPE_CHANGE_H
#define ZONE_TYPE_CHANGE_H

#include "Common.h"
#include "ZoneType.h"

typedef struct ZoneTypeChange {
	uint16_t x;
	uint16_t y;
	ZoneType type;
} ZoneTypeChange;

#endif	// ZONE_TYPE_CHANGE_H
