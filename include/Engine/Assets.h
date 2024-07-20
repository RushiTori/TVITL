#ifndef ASSETS_H
#define ASSETS_H

#include "Common.h"
#include "ZoneType.h"

extern Texture2D ZoneSheets[0x100];
extern Texture2D BrushButtonSheet;
extern Shader ZoneGridShader;

void LoadAssets();
void UnloadAssets();

void PrepareZoneGridShader(Texture2D neighs, ZoneType zoneType);

#endif	// ASSETS_H
