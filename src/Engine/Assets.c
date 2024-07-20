#include "Assets.h"

#include "ZoneType.h"

#define ZONE_SHEETS_DIR "ressources/images/zones/"
#define UI_SHEETS_DIR "ressources/images/UI/"
#define SHADERS_DIR "ressources/shaders/"

Texture2D ZoneSheets[0x100] = {0};
Texture2D BrushButtonSheet = {0};

Shader ZoneGridShader;
static uint32_t gridWidthLoc;
static uint32_t gridHeightLoc;
static uint32_t zoneNeighsLoc;
static uint32_t wallSheetLoc;
static uint32_t fakewallSheetLoc;
static uint32_t deathwallSheetLoc;
static uint32_t colorwallSheetLoc;
static uint32_t teleporterSheetLoc;
static uint32_t checkpointSheetLoc;
static uint32_t endpointSheetLoc;
static uint32_t keywallSheetLoc;
static uint32_t triggerpointSheetLoc;

void LoadAssets() {
	ZoneSheets[ZONE_WALL] = LoadTexture(ZONE_SHEETS_DIR "Wall.png");
	ZoneSheets[ZONE_FAKEWALL] = LoadTexture(ZONE_SHEETS_DIR "FakeWall.png");
	ZoneSheets[ZONE_DEATHWALL] = LoadTexture(ZONE_SHEETS_DIR "DeathWall.png");
	ZoneSheets[ZONE_COLORWALL] = LoadTexture(ZONE_SHEETS_DIR "ColorWall.png");
	ZoneSheets[ZONE_TELEPORTER] = LoadTexture(ZONE_SHEETS_DIR "Teleporter.png");
	ZoneSheets[ZONE_CHECKPOINT] = LoadTexture(ZONE_SHEETS_DIR "CheckPoint.png");
	ZoneSheets[ZONE_ENDPOINT] = LoadTexture(ZONE_SHEETS_DIR "EndPoint.png");
	ZoneSheets[ZONE_KEYWALL] = LoadTexture(ZONE_SHEETS_DIR "KeyWall.png");
	ZoneSheets[ZONE_TRIGGERPOINT] = LoadTexture(ZONE_SHEETS_DIR "TriggerPoint.png");

	BrushButtonSheet = LoadTexture(UI_SHEETS_DIR "BrushButtons.png");

	ZoneGridShader = LoadShader(NULL, SHADERS_DIR "ZoneGrid.fs");
	gridWidthLoc = GetShaderLocation(ZoneGridShader, "gridWidth");
	gridHeightLoc = GetShaderLocation(ZoneGridShader, "gridHeight");
	zoneNeighsLoc = GetShaderLocation(ZoneGridShader, "zoneNeighs");
	wallSheetLoc = GetShaderLocation(ZoneGridShader, "wallSheet");
	fakewallSheetLoc = GetShaderLocation(ZoneGridShader, "fakewallSheet");
	deathwallSheetLoc = GetShaderLocation(ZoneGridShader, "deathwallSheet");
	colorwallSheetLoc = GetShaderLocation(ZoneGridShader, "colorwallSheet");
	teleporterSheetLoc = GetShaderLocation(ZoneGridShader, "teleporterSheet");
	checkpointSheetLoc = GetShaderLocation(ZoneGridShader, "checkpointSheet");
	endpointSheetLoc = GetShaderLocation(ZoneGridShader, "endpointSheet");
	keywallSheetLoc = GetShaderLocation(ZoneGridShader, "keywallSheet");
	triggerpointSheetLoc = GetShaderLocation(ZoneGridShader, "triggerpointSheet");
}

void UnloadAssets() {
	UnloadTexture(ZoneSheets[ZONE_WALL]);
	UnloadTexture(ZoneSheets[ZONE_FAKEWALL]);
	UnloadTexture(ZoneSheets[ZONE_DEATHWALL]);
	UnloadTexture(ZoneSheets[ZONE_COLORWALL]);
	UnloadTexture(ZoneSheets[ZONE_TELEPORTER]);
	UnloadTexture(ZoneSheets[ZONE_CHECKPOINT]);
	UnloadTexture(ZoneSheets[ZONE_ENDPOINT]);
	UnloadTexture(ZoneSheets[ZONE_KEYWALL]);
	UnloadTexture(ZoneSheets[ZONE_TRIGGERPOINT]);
	UnloadTexture(BrushButtonSheet);
	UnloadShader(ZoneGridShader);
}

void PrepareZoneGridShader(Texture2D neighs) {
	SetShaderValue(ZoneGridShader, gridWidthLoc, &neighs.width, SHADER_UNIFORM_INT);
	SetShaderValue(ZoneGridShader, gridHeightLoc, &neighs.height, SHADER_UNIFORM_INT);
	SetShaderValueTexture(ZoneGridShader, zoneNeighsLoc, neighs);
	SetShaderValueTexture(ZoneGridShader, wallSheetLoc, ZoneSheets[ZONE_WALL]);
	SetShaderValueTexture(ZoneGridShader, fakewallSheetLoc, ZoneSheets[ZONE_FAKEWALL]);
	
	SetShaderValueTexture(ZoneGridShader, deathwallSheetLoc, ZoneSheets[ZONE_DEATHWALL]);
	SetShaderValueTexture(ZoneGridShader, colorwallSheetLoc, ZoneSheets[ZONE_COLORWALL]);
	SetShaderValueTexture(ZoneGridShader, teleporterSheetLoc, ZoneSheets[ZONE_TELEPORTER]);
	SetShaderValueTexture(ZoneGridShader, checkpointSheetLoc, ZoneSheets[ZONE_CHECKPOINT]);
	SetShaderValueTexture(ZoneGridShader, endpointSheetLoc, ZoneSheets[ZONE_ENDPOINT]);
	SetShaderValueTexture(ZoneGridShader, keywallSheetLoc, ZoneSheets[ZONE_KEYWALL]);
	SetShaderValueTexture(ZoneGridShader, triggerpointSheetLoc, ZoneSheets[ZONE_TRIGGERPOINT]);
}

Rectangle GetZoneTexOffsets(uint8_t zoneNeighbors, bool isSheetTrimmed) {
	static const uint8_t ZoneNeighborsToOffset[0x100] = {
		[0x00] = 0,	 [0x01] = 0,  [0x02] = 1,  [0x03] = 1,	[0x04] = 0,	 [0x05] = 0,  [0x06] = 1,  [0x07] = 1,
		[0x08] = 2,	 [0x09] = 2,  [0x0A] = 3,  [0x0B] = 4,	[0x0C] = 2,	 [0x0D] = 2,  [0x0E] = 3,  [0x0F] = 4,
		[0x10] = 5,	 [0x11] = 5,  [0x12] = 6,  [0x13] = 6,	[0x14] = 5,	 [0x15] = 5,  [0x16] = 7,  [0x17] = 7,
		[0x18] = 8,	 [0x19] = 8,  [0x1A] = 9,  [0x1B] = 10, [0x1C] = 8,	 [0x1D] = 8,  [0x1E] = 11, [0x1F] = 12,
		[0x20] = 0,	 [0x21] = 0,  [0x22] = 1,  [0x23] = 1,	[0x24] = 0,	 [0x25] = 0,  [0x26] = 1,  [0x27] = 1,
		[0x28] = 2,	 [0x29] = 2,  [0x2A] = 3,  [0x2B] = 4,	[0x2C] = 2,	 [0x2D] = 2,  [0x2E] = 3,  [0x2F] = 4,
		[0x30] = 5,	 [0x31] = 5,  [0x32] = 6,  [0x33] = 6,	[0x34] = 5,	 [0x35] = 5,  [0x36] = 7,  [0x37] = 7,
		[0x38] = 8,	 [0x39] = 8,  [0x3A] = 9,  [0x3B] = 10, [0x3C] = 8,	 [0x3D] = 8,  [0x3E] = 11, [0x3F] = 12,
		[0x40] = 13, [0x41] = 13, [0x42] = 14, [0x43] = 14, [0x44] = 13, [0x45] = 13, [0x46] = 14, [0x47] = 14,
		[0x48] = 15, [0x49] = 15, [0x4A] = 16, [0x4B] = 17, [0x4C] = 15, [0x4D] = 15, [0x4E] = 16, [0x4F] = 17,
		[0x50] = 18, [0x51] = 18, [0x52] = 19, [0x53] = 19, [0x54] = 18, [0x55] = 18, [0x56] = 20, [0x57] = 20,
		[0x58] = 21, [0x59] = 21, [0x5A] = 22, [0x5B] = 23, [0x5C] = 21, [0x5D] = 21, [0x5E] = 24, [0x5F] = 25,
		[0x60] = 13, [0x61] = 13, [0x62] = 14, [0x63] = 14, [0x64] = 13, [0x65] = 13, [0x66] = 14, [0x67] = 14,
		[0x68] = 26, [0x69] = 26, [0x6A] = 27, [0x6B] = 28, [0x6C] = 26, [0x6D] = 26, [0x6E] = 27, [0x6F] = 28,
		[0x70] = 18, [0x71] = 18, [0x72] = 19, [0x73] = 19, [0x74] = 18, [0x75] = 18, [0x76] = 20, [0x77] = 20,
		[0x78] = 29, [0x79] = 29, [0x7A] = 30, [0x7B] = 31, [0x7C] = 29, [0x7D] = 29, [0x7E] = 32, [0x7F] = 33,
		[0x80] = 0,	 [0x81] = 0,  [0x82] = 1,  [0x83] = 1,	[0x84] = 0,	 [0x85] = 0,  [0x86] = 1,  [0x87] = 1,
		[0x88] = 2,	 [0x89] = 2,  [0x8A] = 3,  [0x8B] = 4,	[0x8C] = 2,	 [0x8D] = 2,  [0x8E] = 3,  [0x8F] = 4,
		[0x90] = 5,	 [0x91] = 5,  [0x92] = 6,  [0x93] = 6,	[0x94] = 5,	 [0x95] = 5,  [0x96] = 7,  [0x97] = 7,
		[0x98] = 8,	 [0x99] = 8,  [0x9A] = 9,  [0x9B] = 10, [0x9C] = 8,	 [0x9D] = 8,  [0x9E] = 11, [0x9F] = 12,
		[0xA0] = 0,	 [0xA1] = 0,  [0xA2] = 1,  [0xA3] = 1,	[0xA4] = 0,	 [0xA5] = 0,  [0xA6] = 1,  [0xA7] = 1,
		[0xA8] = 2,	 [0xA9] = 2,  [0xAA] = 3,  [0xAB] = 4,	[0xAC] = 2,	 [0xAD] = 2,  [0xAE] = 3,  [0xAF] = 4,
		[0xB0] = 5,	 [0xB1] = 5,  [0xB2] = 6,  [0xB3] = 6,	[0xB4] = 5,	 [0xB5] = 5,  [0xB6] = 7,  [0xB7] = 7,
		[0xB8] = 8,	 [0xB9] = 8,  [0xBA] = 9,  [0xBB] = 10, [0xBC] = 8,	 [0xBD] = 8,  [0xBE] = 11, [0xBF] = 12,
		[0xC0] = 13, [0xC1] = 13, [0xC2] = 14, [0xC3] = 14, [0xC4] = 13, [0xC5] = 13, [0xC6] = 14, [0xC7] = 14,
		[0xC8] = 15, [0xC9] = 15, [0xCA] = 16, [0xCB] = 17, [0xCC] = 15, [0xCD] = 15, [0xCE] = 16, [0xCF] = 17,
		[0xD0] = 34, [0xD1] = 34, [0xD2] = 35, [0xD3] = 35, [0xD4] = 34, [0xD5] = 34, [0xD6] = 36, [0xD7] = 36,
		[0xD8] = 37, [0xD9] = 37, [0xDA] = 38, [0xDB] = 39, [0xDC] = 37, [0xDD] = 37, [0xDE] = 40, [0xDF] = 41,
		[0xE0] = 13, [0xE1] = 13, [0xE2] = 14, [0xE3] = 14, [0xE4] = 13, [0xE5] = 13, [0xE6] = 14, [0xE7] = 14,
		[0xE8] = 26, [0xE9] = 26, [0xEA] = 27, [0xEB] = 28, [0xEC] = 26, [0xED] = 26, [0xEE] = 27, [0xEF] = 28,
		[0xF0] = 34, [0xF1] = 34, [0xF2] = 35, [0xF3] = 35, [0xF4] = 34, [0xF5] = 34, [0xF6] = 36, [0xF7] = 36,
		[0xF8] = 42, [0xF9] = 42, [0xFA] = 43, [0xFB] = 44, [0xFC] = 42, [0xFD] = 42, [0xFE] = 45, [0xFF] = 46,
	};

	if (!isSheetTrimmed) {
		return (Rectangle){
			.x = (zoneNeighbors & 0b00001111) * WORLD_SCALE,
			.y = (zoneNeighbors >> 4) * WORLD_SCALE,
			.width = WORLD_SCALE,
			.height = WORLD_SCALE,
		};
	}

	uint8_t offset = ZoneNeighborsToOffset[zoneNeighbors];

	return (Rectangle){
		.x = (offset % 7) * WORLD_SCALE,
		.y = (offset / 7) * WORLD_SCALE,
		.width = WORLD_SCALE,
		.height = WORLD_SCALE,
	};
}
