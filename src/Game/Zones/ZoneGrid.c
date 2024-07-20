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

	uint32_t zoneCount = width * height;

	grid.zoneTypes = malloc(zoneCount * sizeof(ZoneType));
	if (!grid.zoneTypes) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zoneTypes\n");
		return (ZoneGrid){0};
	}
	memset(grid.zoneTypes, ZONE_EMPTY, zoneCount * sizeof(ZoneType));

	grid.zoneNeighs = calloc(zoneCount, sizeof(uint8_t));
	if (!grid.zoneNeighs) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zoneNeighs\n");
		return (ZoneGrid){0};
	}

	grid.zoneGroupIDs = calloc(zoneCount, sizeof(uint16_t));
	if (!grid.zoneGroupIDs) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zoneGroupIDs\n");
		return (ZoneGrid){0};
	}

	grid.zoneTypesTex = LoadRenderTexture(width, height);
	BeginTextureMode(grid.zoneTypesTex);
	ClearBackground((Color){
		.r = ZONE_EMPTY,
		.g = ZONE_EMPTY,
		.b = ZONE_EMPTY,
		.a = 0xFF,
	});
	EndTextureMode();

	grid.zoneNeighsTex = LoadRenderTexture(width, height);
	BeginTextureMode(grid.zoneNeighsTex);
	ClearBackground(BLACK);
	EndTextureMode();

	grid.groups.data = DefaultZoneGroups();
	grid.groups.size = DEFAULT_ZONE_GROUPS_SIZE;
	grid.groups.capacity = DEFAULT_ZONE_GROUPS_SIZE;

	return grid;
}

ZoneGrid CreateZoneGridFromFileData(ByteBuffer* buffer) {
	ZoneGrid grid = {0};
	grid.width = ByteBufferRead(uint16_t, buffer);
	grid.height = ByteBufferRead(uint16_t, buffer);
	uint32_t zoneCount = grid.width * grid.height;

	grid.zoneTypes = malloc(zoneCount * sizeof(ZoneType));
	if (!grid.zoneTypes) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zoneTypes\n");
		return (ZoneGrid){0};
	}
	memset(grid.zoneTypes, ZONE_EMPTY, zoneCount * sizeof(ZoneType));

	grid.zoneNeighs = malloc(zoneCount * sizeof(uint8_t));
	if (!grid.zoneNeighs) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zoneNeighs\n");
		return (ZoneGrid){0};
	}

	grid.zoneGroupIDs = malloc(zoneCount * sizeof(uint16_t));
	if (!grid.zoneGroupIDs) {
		TraceLog(LOG_FATAL, "Couldn't allocate grid.zoneGroupIDs\n");
		return (ZoneGrid){0};
	}

	uint32_t zoneIdx = 0;
	while (zoneIdx < zoneCount && buffer->dataPtr < buffer->capacity) {
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
		for (uint32_t i = 0; i < zoneCount && zoneIdx < zoneCount; i++, zoneIdx++) {
			grid.zoneTypes[zoneIdx] = zoneData.type;
			grid.zoneGroupIDs[zoneIdx] = zoneData.groupID;
		}
	}

	for (uint32_t i = 0; i < (grid.width * grid.height); i++) {
		grid.zoneNeighs[i] = GetZoneNeighbors(&grid, i % grid.width, i / grid.width);
	}

	Texture2D bufferTex;
	{
		Image tmpImg;
		tmpImg.width = grid.width;
		tmpImg.height = grid.height;
		tmpImg.mipmaps = 1;
		tmpImg.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
		tmpImg.data = grid.zoneTypes;
		bufferTex = LoadTextureFromImage(tmpImg);
	}

	grid.zoneTypesTex = LoadRenderTexture(grid.width, grid.height);
	BeginTextureMode(grid.zoneTypesTex);
	DrawTexture(bufferTex, 0, 0, WHITE);
	EndTextureMode();

	UpdateTexture(bufferTex, grid.zoneNeighs);

	grid.zoneNeighsTex = LoadRenderTexture(grid.width, grid.height);
	BeginTextureMode(grid.zoneNeighsTex);
	DrawTexture(bufferTex, 0, 0, WHITE);
	EndTextureMode();

	UnloadTexture(bufferTex);

	// WIP
	grid.groups.data = DefaultZoneGroups();
	grid.groups.size = DEFAULT_ZONE_GROUPS_SIZE;
	grid.groups.capacity = DEFAULT_ZONE_GROUPS_SIZE;

	return grid;
}

void FreeZoneGrid(ZoneGrid* grid) {
	free(grid->zoneTypes);
	free(grid->zoneNeighs);
	free(grid->zoneGroupIDs);

	MapDynamicArray(&grid->groups, FreeZoneGroup);
	FreeDynamicArray(&grid->groups);

	UnloadRenderTexture(grid->zoneTypesTex);
	UnloadRenderTexture(grid->zoneNeighsTex);
}

void SerializeZoneGrid(const ZoneGrid* grid, ByteBuffer* buffer) {
	// WIP
	ByteBufferPush(buffer, grid->width);
	ByteBufferPush(buffer, grid->height);

	uint32_t zonesLen = grid->width * grid->height;
	for (uint32_t i = 0; i < zonesLen; i++) {
		ZoneType typeData = grid->zoneTypes[i];
		uint16_t groupIDdata = grid->zoneGroupIDs[i];
		bool evilData = false;	// WIP : modify the save file format

		uint32_t zoneCount = 1;

		while ((i < zonesLen) && (grid->zoneTypes[i] == typeData) && (grid->zoneGroupIDs[i] == groupIDdata) &&
			   (false == evilData)) {
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
	return grid->zoneTypes[GetIndex1D(x, y, grid->width)];
}

void SetZoneType(ZoneGrid* grid, uint32_t x, uint32_t y, ZoneType type) {
	if (x >= grid->width || y >= grid->height) return;
	grid->zoneTypes[GetIndex1D(x, y, grid->width)] = type;

	uint32_t pixelX = x;
	uint32_t pixelY = grid->height - 1 - y;
	BeginTextureMode(grid->zoneTypesTex);
	DrawPixel(pixelX, pixelY,
			  (Color){
				  .r = type,
				  .g = type,
				  .b = type,
				  .a = 0xFF,
			  });
	printf("drew : %u at {%u, %u}.\n", type, pixelX, pixelY);
	EndTextureMode();

	BeginTextureMode(grid->zoneNeighsTex);
	for (int32_t j = -1; j <= 1; j++) {
		uint32_t nY = y + j;
		if (nY >= grid->height) continue;
		for (int32_t i = -1; i <= 1; i++) {
			uint32_t nX = x + i;
			if (nX >= grid->width) continue;

			uint8_t neighs = GetZoneNeighbors(grid, nX, nY);
			grid->zoneNeighs[GetIndex1D(nX, nY, grid->width)] = neighs;

			pixelX = nX;
			pixelY = grid->height - 1 - nY;
			DrawPixel(pixelX, pixelY,
					  (Color){
						  .r = neighs,
						  .g = neighs,
						  .b = neighs,
						  .a = 0xFF,
					  });
		}
	}
	EndTextureMode();
}

void SetPlayerCollidedZone(ZoneGrid* grid, uint32_t x, uint32_t y) {
	if (x >= grid->width || y >= grid->height) return;
	grid->groups.data[grid->zoneGroupIDs[GetIndex1D(x, y, grid->width)]].hasCollided = true;
}

void DisplayZoneGrid(const ZoneGrid* grid) {
	// WIP
	DrawRectangle(0, 0, grid->width * WORLD_SCALE, grid->height * WORLD_SCALE, GRAY);
	BeginShaderMode(ZoneGridShader);
	PrepareZoneGridShader(grid->zoneNeighsTex.texture);

	rlSetTexture(grid->zoneTypesTex.texture.id);
	rlBegin(RL_QUADS);
	rlColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	rlNormal3f(0.0f, 0.0f, 1.0f);

	rlTexCoord2f(0, 0);
	rlVertex2f(0, 0);

	rlTexCoord2f(0, grid->height);
	rlVertex2f(0, WORLD_SCALE * grid->height);

	rlTexCoord2f(grid->width, grid->height);
	rlVertex2f(WORLD_SCALE * grid->width, WORLD_SCALE * grid->height);

	rlTexCoord2f(grid->width, 0);
	rlVertex2f(WORLD_SCALE * grid->width, 0);

	rlEnd();
	rlSetTexture(0);

	EndShaderMode();
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
