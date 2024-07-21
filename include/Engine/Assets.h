#ifndef ASSETS_H
#define ASSETS_H

#include "Common.h"

extern Texture2D ZoneSheets[0x100];
extern Texture2D EntitySheet;
extern Texture2D BrushButtonSheet;

void LoadAssets();
void UnloadAssets();

Rectangle GetZoneTexOffsets(uint8_t zoneNeighbors, bool isSheetTrimmed);

#endif	// ASSETS_H
