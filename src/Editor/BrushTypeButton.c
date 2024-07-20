#include "BrushTypeButton.h"

void DisplayBrushTypeButton(const BrushTypeButton* button) {
	Rectangle texSrc = (Rectangle){
		.x = button->type % 4 * 32,
		.y = button->type / 4 * 32,
		.width = 32,
		.height = 32,
	};

	DrawTexturePro(BrushButtonSheet, texSrc, button->infos.body, (Vector2){0}, 0, WHITE);
	if (button->infos.mouseIn) DrawRectangleRec(button->infos.body, Fade(WHITE, 0.25));
}

void UpdateBrushTypeButton(BrushTypeButton* restrict button, BrushType* restrict typeRef) {
	UpdateButtonInfos(&button->infos);
	if (button->infos.pressedNow && button->infos.mouseIn) *typeRef = button->type;
}
