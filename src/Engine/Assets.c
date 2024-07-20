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
static uint32_t zoneSheetLoc;
static uint32_t activeTypeLoc;

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
	zoneSheetLoc = GetShaderLocation(ZoneGridShader, "zoneSheet");
	activeTypeLoc = GetShaderLocation(ZoneGridShader, "activeZoneType");
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

void PrepareZoneGridShader(Texture2D neighs, ZoneType zoneType) {
	SetShaderValue(ZoneGridShader, gridWidthLoc, &neighs.width, SHADER_UNIFORM_INT);
	SetShaderValue(ZoneGridShader, gridHeightLoc, &neighs.height, SHADER_UNIFORM_INT);
	SetShaderValueTexture(ZoneGridShader, zoneNeighsLoc, neighs);
	SetShaderValueTexture(ZoneGridShader, zoneSheetLoc, ZoneSheets[zoneType]);
	float actType = zoneType / 255.0f;
	SetShaderValue(ZoneGridShader, activeTypeLoc, &actType, SHADER_UNIFORM_FLOAT);
}
