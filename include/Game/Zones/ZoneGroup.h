#ifndef ZONE_GROUP_H
#define ZONE_GROUP_H

#include "ByteBuffer.h"
#include "Common.h"
#include "ZoneType.h"

typedef struct ZoneGroup {
	ZoneType type;
	bool hasCollided;
	ByteBuffer infos;
} ZoneGroup;

ZoneGroup CreateZoneGroup(ZoneType groupType);
void FreeZoneGroup(ZoneGroup* group);

#endif	// ZONE_GROUP_H
