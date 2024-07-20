#include "ButtonInfos.h"

ButtonInfos CreateButtonInfos(float x, float y, float w, float h, bool isInWorld) {
	return CreateButtonInfosRec(
		(Rectangle){
			.x = x,
			.y = y,
			.width = w,
			.height = h,
		},
		isInWorld);
}

ButtonInfos CreateButtonInfosV(Vector2 pos, Vector2 sizes, bool isInWorld) {
	return CreateButtonInfosRec(
		(Rectangle){
			.x = pos.x,
			.y = pos.y,
			.width = sizes.x,
			.height = sizes.y,
		},
		isInWorld);
}

ButtonInfos CreateButtonInfosRec(Rectangle rec, bool isInWorld) {
	ButtonInfos button = {0};
	button.body = rec;
	button.inWorldButton = isInWorld;
	return button;
}

void SetButtonInfosInputs(ButtonInfos* button, KeyboardKey key, MouseButton mButton, GamepadButton gButton) {
	button->shortcutKey = key;
	button->mouseButton = mButton;
	button->gamepadButton = gButton;
}

void UpdateButtonInfos(ButtonInfos* button) {
	// WIP
	Vector2 mousePos = GetMousePosition();
	if (button->inWorldButton) mousePos = GetScreenToGameWorld(mousePos);
	button->mouseIn = CheckCollisionPointRec(mousePos, button->body);

	bool pressed = false;
	pressed |= IsKeyPressed(button->shortcutKey);
	pressed |= IsMouseButtonPressed(button->mouseButton);
	pressed |= IsGamepadButtonPressed(USED_GAMEPAD_ID, button->gamepadButton);

	bool down = false;
	down |= IsKeyDown(button->shortcutKey);
	down |= IsMouseButtonDown(button->mouseButton);
	down |= IsGamepadButtonDown(USED_GAMEPAD_ID, button->gamepadButton);

	bool released = false;
	released |= IsKeyReleased(button->shortcutKey);
	released |= IsMouseButtonReleased(button->mouseButton);
	released |= IsGamepadButtonReleased(USED_GAMEPAD_ID, button->gamepadButton);

	button->pressedPrev = button->pressedNow;
	button->heldDownPrev = button->heldDownNow;
	button->releasedPrev = button->releasedNow;
	
	button->pressedNow = !released && (!button->pressedPrev && pressed);
	button->heldDownNow = !released && ((button->heldDownPrev) || (button->pressedPrev && pressed) || (down));
	button->releasedNow = released;
}
