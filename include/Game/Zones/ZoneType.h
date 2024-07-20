#ifndef ZONE_TYPE_H
#define ZONE_TYPE_H

#include "Common.h"

typedef uint8_t ZoneType;

#define ZONE_EMPTY ((ZoneType)'.')
#define ZONE_WALL ((ZoneType)'#')
#define ZONE_FAKEWALL ((ZoneType)'0')
#define ZONE_DEATHWALL ((ZoneType)'x')
#define ZONE_COLORWALL ((ZoneType)'c')
#define ZONE_TELEPORTER ((ZoneType)'o')
#define ZONE_CHECKPOINT ((ZoneType)'*')
#define ZONE_ENDPOINT ((ZoneType)'+')
#define ZONE_KEYWALL ((ZoneType)'=')
#define ZONE_TRIGGERPOINT ((ZoneType)'%')
#define ZONE_TYPE_COUNT 10

#define IsZoneTypeSolid(type)            \
	({                                   \
		ZoneType tmp = (ZoneType)(type); \
		bool solid = false;              \
		solid |= tmp == ZONE_WALL;       \
		solid |= tmp == ZONE_DEATHWALL;  \
		solid |= tmp == ZONE_KEYWALL;    \
		solid;                           \
	})

#endif	// ZONE_TYPE_H
