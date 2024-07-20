#include "Editor.h"

static inline void InitEditorZoneButtons(Editor* editor) {
	ZoneType zoneTypes[ZONE_TYPE_COUNT] = {
		ZONE_EMPTY,	  ZONE_WALL,	   ZONE_FAKEWALL,	ZONE_DEATHWALL, ZONE_COLORWALL,
		ZONE_KEYWALL, ZONE_TELEPORTER, ZONE_CHECKPOINT, ZONE_ENDPOINT,	ZONE_TRIGGERPOINT,
	};

	float yOff = (100 - 64) / 2.0 - 3;
	float xOff = yOff;

	for (uint32_t i = 0; i < ZONE_TYPE_COUNT; i++) {
		editor->typeButtons[i].infos = CreateButtonInfos(xOff, yOff, 32, 32, false);
		editor->typeButtons[i].type = zoneTypes[i];
		xOff += 32 + 6;
		if (i == 4) {
			xOff = yOff;
			yOff += 32 + 6;
		}
	}
}

static inline void InitEditorBrushButtons(Editor* editor) {
	BrushType brushTypes[BRUSH_TYPE_COUNT] = {
		BRUSH_DIAMOND,		BRUSH_CIRCLE,	   BRUSH_SQUARE,	  BRUSH_RECT_FILL,	  BRUSH_BUCKET,
		BRUSH_LINE_DIAMOND, BRUSH_LINE_CIRCLE, BRUSH_LINE_SQUARE, BRUSH_RECT_CONTOUR, BRUSH_SELECT,
	};

	float yOff = (100 - 64) / 2.0 - 3;
	float xOff = GetScreenWidth() - yOff - 32 - ((32 + 6) * 4);

	for (uint32_t i = 0; i < BRUSH_TYPE_COUNT; i++) {
		editor->brushButtons[i].infos = CreateButtonInfos(xOff, yOff, 32, 32, false);
		editor->brushButtons[i].type = brushTypes[i];
		xOff += 32 + 6;
		if (i == 4) {
			xOff = GetScreenWidth() - yOff - 32 - ((32 + 6) * 4);
			yOff += 32 + 6;
		}
	}
}

Editor CreateEditor() {
	// WIP
	Editor editor = {0};
	editor.mouseType = ZONE_WALL;

	editor.selectPos = (Vector2){
		.x = nanf(""),
		.y = nanf(""),
	};

	editor.zoneChanges = CreateDynamicArray(ZoneTypeChange);
	InitEditorZoneButtons(&editor);
	InitEditorBrushButtons(&editor);

	// WIP
	MoveCameraByScreen((Vector2){0, -100});
	editor.brushType = ENTITY_ORBITER;

	return editor;
}

void FreeEditor(Editor* editor) {
	FreeLevel(&editor->level);
	FreeDynamicArray(&editor->zoneChanges);
	free(editor->brushCache);
}

void CreateLevel(Editor* editor, uint32_t width, uint32_t height) {
	free(editor->brushCache);
	FreeLevel(&editor->level);
	editor->level.grid = CreateZoneGridEmpty(width, height);
	editor->brushCache = malloc(sizeof(bool) * editor->level.grid.width * editor->level.grid.height);
	FreeDynamicArray(&editor->zoneChanges);
	ReserveDynamicArray(&editor->zoneChanges, editor->level.grid.width * editor->level.grid.height);
}

void EditLevel(Editor* editor, const char* levelDataPath) {
	free(editor->brushCache);
	FreeLevel(&editor->level);
	editor->level = CreateLevelFromFile(levelDataPath, editor->level.player);
	editor->brushCache = malloc(sizeof(bool) * editor->level.grid.width * editor->level.grid.height);
	FreeDynamicArray(&editor->zoneChanges);
	ReserveDynamicArray(&editor->zoneChanges, editor->level.grid.width * editor->level.grid.height);
}

void SaveLevelAs(const Editor* editor, const char* savePath) {
	ByteBuffer levelData = CreateByteBufferAsArray(1 << 17);
	SerializeZoneGrid(&editor->level.grid, &levelData);
	SaveFileData(savePath, levelData.data, levelData.size);
	FreeByteBuffer(&levelData);
}

bool IsMouseOnEditorGrid(const Editor* editor) {
	const ZoneGrid* editorGrid = &editor->level.grid;
	Rectangle editorGridBounds = (Rectangle){
		.x = 0,
		.y = 0,
		.width = editorGrid->width * WORLD_SCALE,
		.height = editorGrid->height * WORLD_SCALE,
	};

	Vector2 worldMouse = GetScreenToGameWorld(GetMousePosition());

	return CheckCollisionPointRec(worldMouse, editorGridBounds);
}

void ClearEditorBrushCache(Editor* editor) {
	memset(editor->brushCache, 0, sizeof(bool) * editor->level.grid.width * editor->level.grid.height);
	editor->zoneChanges.size = 0;
}

void ApplyEditorZoneChanges(Editor* editor) {
	for (uint32_t i = 0; i < editor->zoneChanges.size; i++) {
		ZoneTypeChange changes = editor->zoneChanges.data[i];
		SetZoneType(&editor->level.grid, changes.x, changes.y, changes.type);
	}
	if (editor->brushType == BRUSH_BUCKET) ClearEditorBrushCache(editor);
}

void DisplayEditorWorld(const Editor* editor) {
	DisplayLevel(&editor->level);
	DisplayEditorZoneBrush(editor);
	DisplayEditorEntityBrush(editor);
}

void DisplayEditorUI(const Editor* editor) {
	// WIP
	DrawRectangle(0, 0, GetScreenWidth(), 100, PINK);
	DrawRectangleLinesEx((Rectangle){0, 0, GetScreenWidth(), 100}, 9, GOLD);

	for (uint32_t i = 0; i < ZONE_TYPE_COUNT; i++) {
		DisplayZoneTypeButton(editor->typeButtons + i);
	}

	for (uint32_t i = 0; i < BRUSH_TYPE_COUNT; i++) {
		DisplayBrushTypeButton(editor->brushButtons + i);
	}
	char buffer[32] = {0};
	snprintf(buffer, sizeof(char) * 32, "Brush size : %" PRId32 "", editor->brushSize + 1);
	Vector2 textSizes = MeasureTextEx(GetFontDefault(), buffer, 20, 2);
	Vector2 textPos = (Vector2){
		.x = GetScreenWidth() / 2.0f - textSizes.x / 2.0f,
		.y = 9 + 6 + 16 - textSizes.y / 2.0f,
	};
	DrawTextEx(GetFontDefault(), buffer, textPos, 20, 2, BLACK);
}

void UpdateEditor(Editor* editor) {
	// WIP

	UpdateEditorZoneBrush(editor);
	UpdateEditorEntityBrush(editor);

	bool shiftDown = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));

	if (shiftDown) {
		int32_t newBrushSize = editor->brushSize - GetMouseWheelMove();
		editor->brushSize = Clamp(newBrushSize, 0x00, 0xFF);
	}

	if (IsKeyPressed(KEY_LEFT)) {
		if (editor->brushType != BRUSH_DIAMOND) editor->brushType--;
		ClearEditorBrushCache(editor);
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		if (editor->brushType != BRUSH_SELECT) editor->brushType++;
		ClearEditorBrushCache(editor);
	}

	if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){
													   .x = 0,
													   .y = 100,
													   .width = GetScreenWidth(),
													   .height = GetScreenHeight() - 100,
												   })) {
		if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
			MoveCameraByScreen(Vector2Scale(GetMouseDelta(), -1));
		}

		if (!shiftDown) ZoomToScreen(GetMousePosition(), 1 + 0.1 * GetMouseWheelMove());
	}

	for (uint32_t i = 0; i < ZONE_TYPE_COUNT; i++) {
		UpdateZoneTypeButton(editor->typeButtons + i, &editor->mouseType);
	}

	bool wasSelect = (editor->brushType == BRUSH_SELECT);
	for (uint32_t i = 0; i < BRUSH_TYPE_COUNT; i++) {
		UpdateBrushTypeButton(editor->brushButtons + i, &editor->brushType);
	}
	if (wasSelect && (editor->brushType != BRUSH_SELECT)) {
		ApplyEditorZoneChanges(editor);
	}
}
