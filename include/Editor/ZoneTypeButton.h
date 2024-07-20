#ifndef ZONE_TYPE_BUTTON_H
#define ZONE_TYPE_BUTTON_H

#include "Assets.h"
#include "ButtonInfos.h"
#include "Common.h"
#include "EditorBrushType.h"
#include "ZoneType.h"

typedef struct ZoneTypeButton {
	ButtonInfos infos;
	ZoneType type;
} ZoneTypeButton;

void DisplayZoneTypeButton(const ZoneTypeButton* button);
void UpdateZoneTypeButton(ZoneTypeButton* restrict button, ZoneType* restrict typeRef);

#endif	// ZONE_TYPE_BUTTON_H
