#include "ZoneGrid.h"

#define DEFAULT_ZONE_GROUPS_SIZE (1 + 1 + 8)

ZoneGroup* DefaultZoneGroups() {
	ZoneGroup* defaultGroups = malloc(sizeof(ZoneGroup) * DEFAULT_ZONE_GROUPS_SIZE);
	if (!defaultGroups) {
		TraceLog(LOG_FATAL, "Couldn't allocate defaultGroups\n");
		return NULL;
	}

	defaultGroups[0] = CreateZoneGroup(ZONE_EMPTY);
	defaultGroups[1] = CreateZoneGroup(ZONE_DEATHWALL);

	for (uint32_t i = 0; i < 8; i++) {
		defaultGroups[2 + i] = CreateZoneGroup(ZONE_COLORWALL);
		ByteBufferPush(&defaultGroups[2 + i].infos, (uint8_t)i);
	}

	return defaultGroups;
}

static inline uint8_t GetZoneNeighbors(const ZoneGrid* grid, uint32_t x, uint32_t y) {
	uint8_t neighs = 0;
	for (int32_t j = -1; j <= 1; j++) {
		for (int32_t i = -1; i <= 1; i++) {
			if (i == 0 && j == 0) continue;
			neighs <<= 1;
			neighs |= (GetZoneType(grid, x + i, y + j) != ZONE_EMPTY);
		}
	}
	return neighs;
}

ZoneGrid CreateZoneGridEmpty(uint32_t width, uint32_t height) {
	ZoneGrid grid;
	grid.width = width;
	grid.height = height;

	grid.zones = malloc(width * height * sizeof(Zone));
	if (!grid.zones) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zones\n");
		return (ZoneGrid){0};
	}

	Zone emptyZone = {0};
	emptyZone.type = ZONE_EMPTY;

	for (uint32_t i = 0; i < (width * height); i++) {
		grid.zones[i] = emptyZone;
	}

	grid.groups.data = DefaultZoneGroups();
	grid.groups.size = DEFAULT_ZONE_GROUPS_SIZE;
	grid.groups.capacity = DEFAULT_ZONE_GROUPS_SIZE;

	return grid;
}

ZoneGrid CreateZoneGridFromFileData(ByteBuffer* buffer) {
	ZoneGrid grid = {0};
	grid.width = ByteBufferRead(uint16_t, buffer);
	grid.height = ByteBufferRead(uint16_t, buffer);
	uint32_t zonesLen = grid.width * grid.height;

	grid.zones = malloc(grid.width * grid.height * sizeof(Zone));
	if (!grid.zones) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zones\n");
		return (ZoneGrid){0};
	}

	uint32_t zoneIdx = 0;
	while (zoneIdx < zonesLen && buffer->dataPtr < buffer->capacity) {
		Zone zoneData = {0};

		uint32_t zoneCount = ByteBufferRead(uint16_t, buffer);
		zoneData.isEvil = zoneCount & 2;

		if (zoneCount & 1) {
			buffer->dataPtr -= sizeof(uint16_t);
			zoneCount = ByteBufferRead(uint32_t, buffer);
		}
		zoneCount >>= 2;

		zoneData.type = ByteBufferRead(ZoneType, buffer);
		zoneData.groupID = ByteBufferRead(uint16_t, buffer);
		for (uint32_t i = 0; i < zoneCount && zoneIdx < zonesLen; i++) {
			grid.zones[zoneIdx++] = zoneData;
		}
	}

	for (uint32_t i = 0; i < (grid.width * grid.height); i++) {
		grid.zones[i].neighs = GetZoneNeighbors(&grid, i % grid.width, i / grid.width);
	}

	// WIP
	grid.groups.data = DefaultZoneGroups();
	grid.groups.size = DEFAULT_ZONE_GROUPS_SIZE;
	grid.groups.capacity = DEFAULT_ZONE_GROUPS_SIZE;

	return grid;
}

void FreeZoneGrid(ZoneGrid* grid) {
	free(grid->zones);
	for (uint32_t i = 0; i < grid->groups.size; i++) {
		FreeZoneGroup(grid->groups.data + i);
	}
	FreeDynamicArray(&grid->groups);
}

void SerializeZoneGrid(const ZoneGrid* grid, ByteBuffer* buffer) {
	// WIP
	ByteBufferPush(buffer, grid->width);
	ByteBufferPush(buffer, grid->height);

	uint32_t zonesLen = grid->width * grid->height;
	for (uint32_t i = 0; i < zonesLen; i++) {
		ZoneType typeData = grid->zones[i].type;
		uint16_t groupIDdata = grid->zones[i].groupID;
		bool evilData = grid->zones[i].isEvil;

		uint32_t zoneCount = 1;

		while ((i < zonesLen) && (grid->zones[i].type == typeData) && (grid->zones[i].groupID == groupIDdata) &&
			   (grid->zones[i].isEvil == evilData)) {
			i++;
			zoneCount++;
		}
		i--;
		zoneCount--;

		if (zoneCount > (UINT16_MAX >> 2)) {
			zoneCount <<= 2;
			zoneCount |= evilData << 1;
			zoneCount |= 1;
			ByteBufferPush(buffer, zoneCount);
		} else {
			zoneCount <<= 2;
			zoneCount |= evilData << 1;
			ByteBufferPush(buffer, (uint16_t)zoneCount);
		}
		ByteBufferPush(buffer, typeData);
		ByteBufferPush(buffer, groupIDdata);
	}
}

ZoneType GetZoneType(const ZoneGrid* grid, uint32_t x, uint32_t y) {
	if (x >= grid->width || y >= grid->height) return ZONE_EMPTY;
	return grid->zones[GetIndex1D(x, y, grid->width)].type;
}

void SetZoneType(const ZoneGrid* grid, uint32_t x, uint32_t y, ZoneType type) {
	if (x >= grid->width || y >= grid->height) return;
	grid->zones[GetIndex1D(x, y, grid->width)].type = type;

	for (int32_t j = -1; j <= 1; j++) {
		uint32_t nY = y + j;
		if (nY >= grid->height) continue;
		for (int32_t i = -1; i <= 1; i++) {
			uint32_t nX = x + i;
			if (nX >= grid->width) continue;
			grid->zones[GetIndex1D(nX, nY, grid->width)].neighs = GetZoneNeighbors(grid, nX, nY);
		}
	}
}

void SetPlayerCollidedZone(ZoneGrid* grid, uint32_t x, uint32_t y) {
	if (x >= grid->width || y >= grid->height) return;
	grid->groups.data[grid->zones[GetIndex1D(x, y, grid->width)].groupID].hasCollided = true;
}

static inline void DrawZone(Vector2 source, Vector2 dest) {
	const float xOverWidth = source.x / 512;
	const float yOverHeight = source.y / 512;
	const float xPlusWidthOver = (source.x + WORLD_SCALE) / 512;
	const float yPlusHeightOver = (source.y + WORLD_SCALE) / 512;
	const float destRight = dest.x + WORLD_SCALE;
	const float destBottom = dest.y + WORLD_SCALE;

	// Top-left corner for texture and quad
	rlTexCoord2f(xOverWidth, yOverHeight);
	rlVertex2f(dest.x, dest.y);

	// Bottom-left corner for texture and quad
	rlTexCoord2f(xOverWidth, yPlusHeightOver);
	rlVertex2f(dest.x, destBottom);

	// Bottom-right corner for texture and quad
	rlTexCoord2f(xPlusWidthOver, yPlusHeightOver);
	rlVertex2f(destRight, destBottom);

	// Top-right corner for texture and quad
	rlTexCoord2f(xPlusWidthOver, yOverHeight);
	rlVertex2f(destRight, dest.y);
}

void DisplayZoneGrid(const ZoneGrid* grid) {
	DrawRectangle(0, 0, grid->width * WORLD_SCALE, grid->height * WORLD_SCALE, GRAY);

	const Vector2 startPos = GetScreenToGameTile((Vector2){
		.x = 0,
		.y = 0,
	});

	const Vector2 endPos = GetScreenToGameTile((Vector2){
		.x = GetScreenWidth(),
		.y = GetScreenHeight(),
	});

	const uint32_t startX = Clamp(startPos.x, 0, grid->width - 1);
	const uint32_t startY = Clamp(startPos.y, 0, grid->height - 1);

	const uint32_t endX = Clamp(endPos.x, 0, grid->width - 1);
	const uint32_t endY = Clamp(endPos.y, 0, grid->height - 1);

	Vector2 drawDest = (Vector2){
		.x = 0,
		.y = startY * WORLD_SCALE,
	};

	uint32_t texID = -1;

	rlBegin(RL_QUADS);
	rlColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	rlNormal3f(0.0f, 0.0f, 1.0f);

	for (register uint32_t y = startY; y <= (uint32_t)endY; y++, drawDest.y += WORLD_SCALE) {
		drawDest.x = startX * WORLD_SCALE;
		for (register uint32_t x = startX; x <= (uint32_t)endX; x++, drawDest.x += WORLD_SCALE) {
			register uint32_t idx = GetIndex1D(x, y, grid->width);
			register ZoneType type = grid->zones[idx].type;
			if (type == ZONE_EMPTY) continue;

			register uint8_t neighs = grid->zones[idx].neighs;

			register Vector2 drawSource = (Vector2){
				.x = WORLD_SCALE * (neighs & 0b00001111),
				.y = WORLD_SCALE * (neighs >> 4),
			};

			if (ZoneSheets[type].id != texID) rlSetTexture(texID = ZoneSheets[type].id);

			DrawZone(drawSource, drawDest);
		}
	}
	rlEnd();
	rlSetTexture(0);
}

void UpdateZoneGrid(ZoneGrid* grid) {
	// WIP
	for (uint32_t i = 0; i < grid->groups.size; i++) {
		// WIP
		ZoneType groupType = grid->groups.data[i].type;
		if (groupType == ZONE_DEATHWALL) {
			// WIP
		}
	}
}
