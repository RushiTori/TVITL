#include "Editor.h"

void DisplayEditorZoneBrush(const Editor* editor) {
	Rectangle worldView = GetWorldView();
	for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
		ZoneTypeChange change = editor->zoneChanges.data[i];

		Rectangle changesBody = (Rectangle){
			.x = change.x * WORLD_SCALE,
			.y = change.y * WORLD_SCALE,
			.width = WORLD_SCALE,
			.height = WORLD_SCALE,
		};

		if (!CheckCollisionRecs(worldView, changesBody)) continue;

		if (change.type == ZONE_EMPTY) {
			DrawRectangleLinesEx(changesBody, 2, RED);
		} else {
			uint8_t neighs = 0;
			for (int32_t j = -1; j <= 1; j++) {
				for (int32_t i = -1; i <= 1; i++) {
					if (i == 0 && j == 0) continue;
					neighs <<= 1;
					neighs |= GetZoneType(&editor->level.grid, change.x + i, change.y + j) != ZONE_EMPTY;
					if (((uint32_t)(change.x + i) < editor->level.grid.width) &
						((uint32_t)(change.y + j) < editor->level.grid.height)) {
						neighs |= editor->brushCache[GetIndex1D(change.x + i, change.y + j, editor->level.grid.width)];
					}
				}
			}
			DrawTexturePro(ZoneSheets[change.type], GetZoneTexOffsets(neighs, false), changesBody, (Vector2){0}, 0,
						   Fade(WHITE, 0.5f));
		}
	}
}

static void TryAddZoneChange(Editor* editor, uint32_t x, uint32_t y, ZoneType type) {
	if (x >= editor->level.grid.width || y >= editor->level.grid.height) return;
	uint32_t cacheIdx = GetIndex1D(x, y, editor->level.grid.width);
	if (editor->brushCache[cacheIdx]) return;
	editor->brushCache[cacheIdx] = true;

	ZoneTypeChange change = (ZoneTypeChange){
		.x = x,
		.y = y,
		.type = type,
	};

	PushBackDynamicArray(&editor->zoneChanges, change);
}

static void FindAffectedZones_DIAMOND(Editor* editor, uint32_t posX, uint32_t posY, int32_t radius) {
	for (int32_t j = -radius; j <= radius; j++) {
		uint32_t y = posY + j;
		if (y >= editor->level.grid.height) continue;
		for (int32_t i = -radius; i <= radius; i++) {
			uint32_t x = posX + i;
			if (x >= editor->level.grid.width) continue;

			if (abs(j) + abs(i) <= radius) TryAddZoneChange(editor, x, y, editor->mouseType);
		}
	}
}

static void FindAffectedZones_CIRCLE(Editor* editor, uint32_t posX, uint32_t posY, int32_t radius) {
	TryAddZoneChange(editor, posX, posY, editor->mouseType);
	Vector2 circCenter = (Vector2){
		.x = posX + 0.5f,
		.y = posY + 0.5f,
	};

	int32_t radiusSqr = radius * radius;

	for (int32_t j = -radius; j <= radius; j++) {
		uint32_t y = posY + j;
		if (y >= editor->level.grid.height) continue;
		for (int32_t i = -radius; i <= radius; i++) {
			uint32_t x = posX + i;
			if (x >= editor->level.grid.width) continue;

			Vector2 zonePos = (Vector2){
				.x = x + 1.0f,
				.y = y + 1.0f,
			};

			if (Vector2DistanceSqr(circCenter, zonePos) < radiusSqr) TryAddZoneChange(editor, x, y, editor->mouseType);
		}
	}
}

static void FindAffectedZones_SQUARE(Editor* editor, uint32_t posX, uint32_t posY, int32_t radius) {
	for (int32_t j = -radius; j <= radius; j++) {
		uint32_t y = posY + j;
		if (y >= editor->level.grid.height) continue;
		for (int32_t i = -radius; i <= radius; i++) {
			uint32_t x = posX + i;
			if (x >= editor->level.grid.width) continue;

			TryAddZoneChange(editor, x, y, editor->mouseType);
		}
	}
}

typedef void (*FindCallBack)(Editor*, uint32_t, uint32_t, int32_t);

static void FindAffectedZonesOnLine(Editor* editor, Vector2 lineA, Vector2 lineB, int32_t r, FindCallBack callBack) {
	int32_t x1 = lineA.x;
	int32_t y1 = lineA.y;
	int32_t x2 = lineB.x;
	int32_t y2 = lineB.y;
	int32_t deltaX = abs(x2 - x1);
	int32_t deltaY = abs(y2 - y1);
	int32_t stepX = (x1 < x2) ? 1 : -1;
	int32_t stepY = (y1 < y2) ? 1 : -1;
	int32_t x = x1;
	int32_t y = y1;

	if (deltaX == 0 && deltaY == 0) {
		callBack(editor, x, y, r);

		return;
	}

	if (deltaX > deltaY) {
		int32_t error = 2 * deltaY - deltaX;
		for (int32_t i = 0, n = deltaX; i <= n; i++) {
			callBack(editor, x, y, r);

			if (error > 0) {
				error -= deltaX * 2;
				y += stepY;
			}
			x += stepX;
			error += deltaY * 2;
		}
	} else {
		int32_t error = 2 * deltaX - deltaY;
		for (int32_t i = 0, n = deltaY; i <= n; i++) {
			callBack(editor, x, y, r);

			if (error > 0) {
				error -= deltaY * 2;
				x += stepX;
			}
			y += stepY;
			error += deltaX * 2;
		}
	}
}

static void FindAffectedZones_LINE(Editor* editor, Vector2 anchor, int32_t r, FindCallBack callBack) {
	callBack(editor, anchor.x, anchor.y, r);

	if (!isnan(editor->selectPos.x) && !isnan(editor->selectPos.y)) {
		FindAffectedZonesOnLine(editor, editor->selectPos, anchor, r, callBack);
	}
}

static void FindAffectedZonesInRect(Editor* editor, Vector2 posA, Vector2 posB) {
	Vector2 posMin = (Vector2){0};
	Vector2 posMax = (Vector2){
		.x = editor->level.grid.width - 1,
		.y = editor->level.grid.height - 1,
	};

	posA = Vector2Clamp(posA, posMin, posMax);
	posB = Vector2Clamp(posB, posMin, posMax);

	uint32_t startX = min(posA.x, posB.x);
	uint32_t startY = min(posA.y, posB.y);

	uint32_t endX = max(posA.x, posB.x);
	uint32_t endY = max(posA.y, posB.y);

	for (uint32_t y = startY; y <= endY; y++) {
		for (uint32_t x = startX; x <= endX; x++) {
			TryAddZoneChange(editor, x, y, editor->mouseType);
		}
	}
}

void UpdateEditorSelectPos(Editor* editor, Vector2 anchor) {
	Vector2 anchorMin = (Vector2){0};
	Vector2 anchorMax = (Vector2){
		.x = editor->level.grid.width - 1,
		.y = editor->level.grid.height - 1,
	};
	anchor = Vector2Clamp(anchor, anchorMin, anchorMax);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		editor->selectPos = anchor;
	} else if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) &&
			   !IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		editor->selectPos = (Vector2){
			.x = nanf(""),
			.y = nanf(""),
		};
	}
}

void UpdateEditorZoneBrush(Editor* editor) {
	if ((!CheckCollisionPointRec(GetScreenToGameWorld(GetMousePosition()),
								 (Rectangle){
									 .x = 0,
									 .y = 0,
									 .width = editor->level.grid.width * WORLD_SCALE,
									 .height = editor->level.grid.height * WORLD_SCALE,
								 })) ||
		(CheckCollisionPointRec(GetMousePosition(), (Rectangle){
														.x = 0,
														.y = 0,
														.width = GetScreenWidth(),
														.height = 100,
													}))) {
		if (editor->brushType != BRUSH_SELECT) ClearEditorBrushCache(editor);
		return;
	}

	ZoneGrid* editorGrid = &editor->level.grid;

	Vector2 mouseZone = GetScreenToGameTile(GetMousePosition());
	Vector2 prevMouseZone = GetScreenToGameTile(Vector2Subtract(GetMousePosition(), GetMouseDelta()));

	uint32_t mouseX = mouseZone.x;
	uint32_t mouseY = mouseZone.y;

	int32_t brushRadius = editor->brushSize;

	UpdateEditorSelectPos(editor, mouseZone);
	bool isSelectValid = (!isnan(editor->selectPos.x) && !isnan(editor->selectPos.y));

	switch (editor->brushType) {
		default:
			ClearEditorBrushCache(editor);
			break;
		case BRUSH_DIAMOND: {
			ClearEditorBrushCache(editor);
			FindAffectedZonesOnLine(editor, prevMouseZone, mouseZone, brushRadius, FindAffectedZones_DIAMOND);
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_CIRCLE: {
			ClearEditorBrushCache(editor);
			FindAffectedZonesOnLine(editor, prevMouseZone, mouseZone, brushRadius, FindAffectedZones_CIRCLE);
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_SQUARE: {
			ClearEditorBrushCache(editor);
			FindAffectedZonesOnLine(editor, prevMouseZone, mouseZone, brushRadius, FindAffectedZones_SQUARE);
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_LINE_DIAMOND: {
			ClearEditorBrushCache(editor);
			FindAffectedZones_LINE(editor, mouseZone, brushRadius, FindAffectedZones_DIAMOND);
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_LINE_CIRCLE: {
			ClearEditorBrushCache(editor);
			FindAffectedZones_LINE(editor, mouseZone, brushRadius, FindAffectedZones_CIRCLE);
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_LINE_SQUARE: {
			ClearEditorBrushCache(editor);
			FindAffectedZones_LINE(editor, mouseZone, brushRadius, FindAffectedZones_SQUARE);
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_RECT_FILL: {
			ClearEditorBrushCache(editor);
			TryAddZoneChange(editor, mouseX, mouseY, editor->mouseType);
			if (isSelectValid) FindAffectedZonesInRect(editor, editor->selectPos, mouseZone);
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_RECT_CONTOUR: {
			if (brushRadius > 0) brushRadius--;
			ClearEditorBrushCache(editor);
			if (!isSelectValid) {
				FindAffectedZonesInRect(editor, mouseZone,
										(Vector2){
											.x = mouseZone.x + brushRadius,
											.y = mouseZone.y + brushRadius,
										});
				break;
			}

			Vector2 rectMin = (Vector2){
				.x = min(mouseZone.x, editor->selectPos.x),
				.y = min(mouseZone.y, editor->selectPos.y),
			};

			Vector2 rectMax = (Vector2){
				.x = max(mouseZone.x, editor->selectPos.x),
				.y = max(mouseZone.y, editor->selectPos.y),
			};

			FindAffectedZonesInRect(editor, rectMin,
									(Vector2){
										.x = rectMax.x,
										.y = min(rectMin.y + brushRadius, rectMax.y),
									});

			FindAffectedZonesInRect(editor, rectMax,
									(Vector2){
										.x = max(rectMax.x - brushRadius, rectMin.x),
										.y = rectMin.y,
									});

			FindAffectedZonesInRect(editor, rectMax,
									(Vector2){
										.x = rectMin.x,
										.y = max(rectMax.y - brushRadius, rectMin.y),
									});

			FindAffectedZonesInRect(editor, rectMin,
									(Vector2){
										.x = min(rectMin.x + brushRadius, rectMax.x),
										.y = rectMax.y,
									});

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

		case BRUSH_BUCKET: {
			if (!editor->brushCache[GetIndex1D(mouseX, mouseY, editor->level.grid.width)]) {
				ClearEditorBrushCache(editor);

				ZoneType targetType = GetZoneType(editorGrid, mouseX, mouseY);
				ZoneType bucketType = editor->mouseType;
				TryAddZoneChange(editor, mouseX, mouseY, bucketType);
				for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
					uint32_t x = editor->zoneChanges.data[i].x;
					uint32_t y = editor->zoneChanges.data[i].y;
					if (GetZoneType(editorGrid, x, y - 1) == targetType) TryAddZoneChange(editor, x, y - 1, bucketType);
					if (GetZoneType(editorGrid, x + 1, y) == targetType) TryAddZoneChange(editor, x + 1, y, bucketType);
					if (GetZoneType(editorGrid, x, y + 1) == targetType) TryAddZoneChange(editor, x, y + 1, bucketType);
					if (GetZoneType(editorGrid, x - 1, y) == targetType) TryAddZoneChange(editor, x - 1, y, bucketType);
				}
			}
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ApplyEditorZoneChanges(editor);
		} break;

			/*case BRUSH_EYE_DROPPER: {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) editor->mouseType = GetZoneType(editorGrid, mouseX,
			mouseY); } break;*/

		case BRUSH_SELECT: {
			// WIP : replace this temp var ASAP
			static bool tmp = false;
			if (tmp) {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					bool mouseInSelection = false;
					// WIP: editor->brushCache[GetIndex1D(mouseX, mouseY, editor->level.grid.width)];
					for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
						if (editor->zoneChanges.data[i].x == mouseX && editor->zoneChanges.data[i].y == mouseY) {
							mouseInSelection = true;
							break;
						}
					}

					if (!mouseInSelection) {
						for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
							uint32_t changeX = editor->zoneChanges.data[i].x;
							uint32_t changeY = editor->zoneChanges.data[i].y;
							ZoneType changeType = editor->zoneChanges.data[i].type;
							if (changeType != ZONE_EMPTY) SetZoneType(editorGrid, changeX, changeY, changeType);
						}
						ClearEditorBrushCache(editor);
					}
				} else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isSelectValid) {
					int32_t deltaX = mouseX - editor->selectPos.x;
					int32_t deltaY = mouseY - editor->selectPos.y;
					if (deltaX || deltaY) {
						// WIP : somehow displace the selection
						uint32_t prevSize = editor->zoneChanges.size;
						ClearEditorBrushCache(editor);
						editor->zoneChanges.size = prevSize;
						for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
							editor->zoneChanges.data[i].x += deltaX;
							editor->zoneChanges.data[i].y += deltaY;
							uint32_t x = editor->zoneChanges.data[i].x;
							uint32_t y = editor->zoneChanges.data[i].y;
							uint32_t editorWidth = editor->level.grid.width;
							uint32_t editorHeight = editor->level.grid.height;
							if (x >= editorWidth || y >= editorHeight) continue;
							if (editor->zoneChanges.data[i].type == ZONE_EMPTY) continue;
							editor->brushCache[GetIndex1D(x, y, editorWidth)] = true;
						}

						editor->selectPos = mouseZone;
					}
				} else {
					if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
						editor->selectPos = mouseZone;
					} else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
						uint32_t startX = min(mouseZone.x, editor->selectPos.x);
						uint32_t startY = min(mouseZone.y, editor->selectPos.y);

						uint32_t endX = max(mouseZone.x, editor->selectPos.x);
						uint32_t endY = max(mouseZone.y, editor->selectPos.y);

						for (uint32_t y = startY; y <= endY; y++) {
							for (uint32_t x = startX; x <= endX; x++) {
								for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
									// WIP : maybe try to find a better way to do this
									uint32_t changeX = editor->zoneChanges.data[i].x;
									uint32_t changeY = editor->zoneChanges.data[i].y;
									ZoneType changeType = editor->zoneChanges.data[i].type;
									if (changeX == x && changeY == y) {
										SetZoneType(editorGrid, x, y, changeType);
										memmove(editor->zoneChanges.data + i, editor->zoneChanges.data + i + 1,
												sizeof(ZoneTypeChange) * (editor->zoneChanges.size - i));
										editor->zoneChanges.size--;
										break;
									} else if (changeType == ZONE_EMPTY) {
										// WIP : not sure about this behaviour, but it does come from Aseprite so I'll
										// leave it like that
										editor->zoneChanges.data[i].type = GetZoneType(editorGrid, changeX, changeY);
										SetZoneType(editorGrid, changeX, changeY, ZONE_EMPTY);
									}
								}
							}
						}
						editor->selectPos = (Vector2){
							.x = nanf(""),
							.y = nanf(""),
						};
					} else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)) {
						// WIP : maybe add more checks to the key down/pressed logic because of Raylib
						for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
							uint32_t changeX = editor->zoneChanges.data[i].x;
							uint32_t changeY = editor->zoneChanges.data[i].y;
							ZoneType changeType = editor->zoneChanges.data[i].type;
							if (changeType != ZONE_EMPTY) SetZoneType(editorGrid, changeX, changeY, changeType);
						}
					}
				}
				tmp = editor->zoneChanges.size > 0;
			} else if (isSelectValid) {
				ClearEditorBrushCache(editor);
				FindAffectedZonesInRect(editor, editor->selectPos, mouseZone);

				for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
					editor->zoneChanges.data[i].type = ZONE_EMPTY;
				}

				if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
					uint32_t prevSize = editor->zoneChanges.size;
					ClearEditorBrushCache(editor);
					editor->zoneChanges.size = prevSize;
					for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
						uint32_t changeX = editor->zoneChanges.data[i].x;
						uint32_t changeY = editor->zoneChanges.data[i].y;
						editor->zoneChanges.data[i].type = GetZoneType(editorGrid, changeX, changeY);
						SetZoneType(editorGrid, changeX, changeY, ZONE_EMPTY);

						uint32_t editorWidth = editor->level.grid.width;
						uint32_t editorHeight = editor->level.grid.height;
						if (changeX >= editorWidth || changeY >= editorHeight) continue;
						if (editor->zoneChanges.data[i].type == ZONE_EMPTY) continue;
						editor->brushCache[GetIndex1D(changeX, changeY, editorWidth)] = true;
					}
					tmp = editor->zoneChanges.size > 0;
				}
			}
		} break;
	}
}
