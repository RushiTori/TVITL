#ifndef BRUSH_TYPE_BUTTON_H
#define BRUSH_TYPE_BUTTON_H

#include "Assets.h"
#include "ButtonInfos.h"
#include "Common.h"
#include "EditorBrushType.h"

typedef struct BrushTypeButton {
	ButtonInfos infos;
	BrushType type;
} BrushTypeButton;

void DisplayBrushTypeButton(const BrushTypeButton* button);
void UpdateBrushTypeButton(BrushTypeButton* restrict button, BrushType* restrict typeRef);

#endif	// BRUSH_TYPE_BUTTON_H
