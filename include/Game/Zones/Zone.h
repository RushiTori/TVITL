#ifndef ZONE_H
#define ZONE_H

#include "Common.h"
#include "ZoneType.h"

typedef struct Zone {
	union {
		uint8_t fullType;
		ZoneType type : 7;
		bool isEvil : 1;
	};
	uint8_t neighs;
	uint16_t groupID;
} Zone;

#endif	// ZONE_H
