#include "Editor.h"

static inline void DrawPotentialLine(Vector2 posA, Vector2 posB, float thick, uint32_t segmentCount, Color col) {
	const float segLen = (Vector2Distance(posA, posB) / (segmentCount * 2)) / Vector2Distance(posA, posB);
	const float segOffset = segLen * 2;
	const float baseOffset = modf(GetTime(), NULL) * segOffset;

	DrawLineEx(Vector2Lerp(posA, posB, 0), Vector2Lerp(posA, posB, max(0, baseOffset - segLen)), thick, col);

	float drawOffsetA = baseOffset;
	float drawOffsetB = baseOffset + segLen;

	for (uint32_t i = 0; i < segmentCount; i++) {
		DrawLineEx(Vector2Lerp(posA, posB, drawOffsetA), Vector2Lerp(posA, posB, drawOffsetB), thick, col);
		drawOffsetA += segOffset;
		drawOffsetB = min(drawOffsetB + segOffset, 1);
	}
}

static inline void DrawPotentialMover(Vector2 pos, Color col) {
	const Vector2 posA = (Vector2){.x = pos.x - MOVER_SIZE / 2, .y = pos.y - MOVER_SIZE / 2};
	const Vector2 posB = (Vector2){.x = pos.x + MOVER_SIZE / 2, .y = pos.y - MOVER_SIZE / 2};
	const Vector2 posC = (Vector2){.x = pos.x + MOVER_SIZE / 2, .y = pos.y + MOVER_SIZE / 2};
	const Vector2 posD = (Vector2){.x = pos.x - MOVER_SIZE / 2, .y = pos.y + MOVER_SIZE / 2};

	DrawPotentialLine((Vector2){.x = posA.x, .y = posA.y + 1}, (Vector2){.x = posB.x, .y = posB.y + 1}, 2, 4, col);
	DrawPotentialLine((Vector2){.x = posB.x - 1, .y = posB.y}, (Vector2){.x = posC.x - 1, .y = posC.y}, 2, 4, col);
	DrawPotentialLine((Vector2){.x = posC.x, .y = posC.y - 1}, (Vector2){.x = posD.x, .y = posD.y - 1}, 2, 4, col);
	DrawPotentialLine((Vector2){.x = posD.x + 1, .y = posD.y}, (Vector2){.x = posA.x + 1, .y = posA.y}, 2, 4, col);
}

static inline void DrawPotentialOrbiter(Vector2 pos, Color col) {
	const uint32_t segmentCount = 10;
	const float segLen = ((2 * PI) / (segmentCount * 2)) * RAD2DEG;
	const float segOffset = segLen * 2;
	const float baseOffset = modf(GetTime(), NULL) * segOffset;
	float angle = baseOffset;
	for (uint32_t i = 0; i < segmentCount; i++) {
		DrawRing(pos, ORBITER_RADIUS - 2, ORBITER_RADIUS, angle, angle + segLen, 2, col);
		angle += segOffset;
	}
}

static inline Vector2 GetEntityPos() {
	Vector2 worldMouse = GetScreenToGameWorld(GetMousePosition());
	if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))
		return (Vector2){
			.x = roundf(worldMouse.x),
			.y = roundf(worldMouse.y),
		};

	return (Vector2){
		.x = roundf(worldMouse.x / (WORLD_SCALE / 2)) * (WORLD_SCALE / 2),
		.y = roundf(worldMouse.y / (WORLD_SCALE / 2)) * (WORLD_SCALE / 2),
	};
}

void DisplayEditorEntityBrush(const Editor* editor) {
	if (!IsMouseOnEditorGrid(editor)) return;
	Vector2 entityPos = GetEntityPos();

	switch (editor->brushType) {
		default:
			break;

		case ENTITY_MOVER: {
			DrawPotentialMover(entityPos, PINK);
		} break;

		case ENTITY_ORBITER: {
			Vector2 dir = (Vector2){
				.x = 1,
				.y = 0,
			};
			Vector2 orbPos = Vector2Add(entityPos, Vector2Scale(dir, WORLD_SCALE * 2));
			DrawPotentialOrbiter(orbPos, PINK);

			Vector2 posB = Vector2Add(entityPos, Vector2Scale(dir, WORLD_SCALE * 2 - ORBITER_RADIUS));
			DrawPotentialLine(entityPos, posB, 2, 6, PINK);

			DrawCircleV(entityPos, 4, PINK);
		} break;

		case ENTITY_MOVORBITER: {
			DrawPotentialMover(entityPos, PINK);
			for (uint32_t i = 0; i < 4; i++) {
				Vector2 dir = (Vector2){
					.x = cos(i * PI / 2),
					.y = sin(i * PI / 2),
				};
				Vector2 orbPos = Vector2Add(entityPos, Vector2Scale(dir, WORLD_SCALE * 2));
				DrawPotentialOrbiter(orbPos, PINK);

				Vector2 posA = Vector2Add(entityPos, Vector2Scale(dir, MOVER_SIZE / 2));
				Vector2 posB = Vector2Add(entityPos, Vector2Scale(dir, (WORLD_SCALE * 2) - ORBITER_RADIUS));
				DrawPotentialLine(posA, posB, 2, 4, PINK);
			}
		} break;

		case ENTITY_SHOOTER: {
			DrawPotentialMover(entityPos, PINK);
		} break;
	}
}

void UpdateEditorEntityBrush(Editor* editor) {
	if (!IsMouseOnEditorGrid(editor)) return;
	Vector2 entityPos = GetEntityPos();

	if (IsKeyPressed(KEY_KP_0)) editor->brushType = ENTITY_MOVER;
	if (IsKeyPressed(KEY_KP_1)) editor->brushType = ENTITY_ORBITER;
	if (IsKeyPressed(KEY_KP_2)) editor->brushType = ENTITY_MOVORBITER;
	if (IsKeyPressed(KEY_KP_3)) editor->brushType = ENTITY_SHOOTER;

	switch (editor->brushType) {
		default:
			break;

		case ENTITY_MOVER: {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				Mover tmp = CreateMover(entityPos);
				PushBackDynamicArray(&editor->level.movers, tmp);
			}
		} break;

		case ENTITY_ORBITER: {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				Orbiter tmp = CreateOrbiter(entityPos, 2, 0, 1);
				PushBackDynamicArray(&editor->level.orbiters, tmp);
			}
		} break;

		case ENTITY_MOVORBITER: {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				for (uint32_t i = 0; i < 250; i++) {
					Movorbiter tmp = CreateMovorbiter(entityPos, 2, 0, 1, 4);
					PushBackDynamicArray(&editor->level.movorbiters, tmp);
				}
				printf("%u\n", editor->level.movorbiters.size);
			}

		} break;

		case ENTITY_SHOOTER: {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				Shooter tmp = CreateShooter(entityPos, 0, 0, 1, 1);
				PushBackDynamicArray(&editor->level.shooters, tmp);
			}
		} break;
	}
}
