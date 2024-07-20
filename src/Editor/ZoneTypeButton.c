#include "ZoneTypeButton.h"

void DisplayZoneTypeButton(const ZoneTypeButton* button) {
	if (button->type == ZONE_EMPTY) {
		// DrawRectangleLinesEx(button->infos.body, 4, RED);
		Rectangle texSrc = (Rectangle){
			.x = BRUSH_TYPE_COUNT % 4 * 32,
			.y = BRUSH_TYPE_COUNT / 4 * 32,
			.width = 32,
			.height = 32,
		};

		DrawTexturePro(BrushButtonSheet, texSrc, button->infos.body, (Vector2){0}, 0, WHITE);
	} else {
		DrawTexturePro(ZoneSheets[button->type], GetZoneTexOffsets(0, false), button->infos.body, (Vector2){0}, 0,
					   WHITE);
	}

	if (button->infos.mouseIn) DrawRectangleRec(button->infos.body, Fade(WHITE, 0.25));
}

void UpdateZoneTypeButton(ZoneTypeButton* restrict button, ZoneType* restrict typeRef) {
	UpdateButtonInfos(&button->infos);
	if (button->infos.pressedNow && button->infos.mouseIn) *typeRef = button->type;
}
