#ifndef BUTTON_INFOS_H
#define BUTTON_INFOS_H

#include "Common.h"
#include "GameCamera.h"

#define USED_GAMEPAD_ID 0

typedef struct ButtonInfos {
	Rectangle body;
	float timeSinceLastPress;
	float timeHeld;
	uint16_t shortcutKey;
	uint8_t mouseButton : 3;
	uint8_t gamepadButton : 5;
	bool inWorldButton : 1;
	bool mouseIn : 1;
	bool pressedPrev : 1;
	bool heldDownPrev : 1;
	bool releasedPrev : 1;
	bool pressedNow : 1;
	bool heldDownNow : 1;
	bool releasedNow : 1;
} ButtonInfos;

ButtonInfos CreateButtonInfos(float x, float y, float w, float h, bool isInWorld);
ButtonInfos CreateButtonInfosV(Vector2 pos, Vector2 sizes, bool isInWorld);
ButtonInfos CreateButtonInfosRec(Rectangle rec, bool isInWorld);

void SetButtonInfosInputs(ButtonInfos* button, KeyboardKey key, MouseButton mButton, GamepadButton gButton);

void UpdateButtonInfos(ButtonInfos* button);

#endif	// BUTTON_INFOS_H
