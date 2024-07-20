#ifndef EDITOR_H
#define EDITOR_H

#include "BrushTypeButton.h"
#include "Common.h"
#include "EditorBrushType.h"
#include "GameCamera.h"
#include "Level.h"
#include "ZoneTypeButton.h"
#include "ZoneTypeChange.h"

typedef struct Editor {
	Level level;
	ZoneTypeButton typeButtons[ZONE_TYPE_COUNT];
	BrushTypeButton brushButtons[BRUSH_TYPE_COUNT];
	bool* brushCache;
	DynamicArray(ZoneTypeChange) zoneChanges;
	BrushType brushType;
	ZoneType mouseType;
	uint8_t brushSize;
	Vector2 selectPos;
} Editor;

Editor CreateEditor();
void FreeEditor(Editor* editor);

void CreateLevel(Editor* editor, uint32_t width, uint32_t height);
void EditLevel(Editor* editor, const char* levelDataPath);
void SaveLevelAs(const Editor* editor, const char* savePath);

bool IsMouseOnEditorGrid(const Editor* editor);

void ClearEditorBrushCache(Editor* editor);
void ApplyEditorZoneChanges(Editor* editor);

void DisplayEditorWorld(const Editor* editor);
void DisplayEditorZoneBrush(const Editor* editor);
void DisplayEditorEntityBrush(const Editor* editor);

void DisplayEditorUI(const Editor* editor);

void UpdateEditor(Editor* editor);
void UpdateEditorZoneBrush(Editor* editor);
void UpdateEditorEntityBrush(Editor* editor);

#endif	// EDITOR_H
