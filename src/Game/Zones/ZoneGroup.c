#include "ZoneGroup.h"

ZoneGroup CreateZoneGroup(ZoneType groupType) {
	ZoneGroup group = {0};
	group.type = groupType;
	group.infos = CreateByteBufferAsArray(0);
	return group;
}

void FreeZoneGroup(ZoneGroup* group) { FreeByteBuffer(&group->infos); }
