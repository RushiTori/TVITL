#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "Common.h"

extern Camera2D gameCamera;

void InitGameCamera();
void UpdateGameCamera(Vector2 focusPoint, float maxPanSpeed);

Rectangle GetWorldView();

Vector2 GetScreenToGameWorld(Vector2 screenPos);
Vector2 GetScreenToGameTile(Vector2 screenPos);
Vector2 GetGameWorldToGameTile(Vector2 worldPos);
Vector2 GetGameWorldToScreen(Vector2 worldPos);

void SetCameraPosScreen(Vector2 screenPos);
void SetCameraPosWorld(Vector2 worldPos);

void MoveCameraByScreen(Vector2 vel);
void MoveCameraByWorld(Vector2 vel);

void PanToScreen(Vector2 screenPos, float panSpeed);
void PanToWorld(Vector2 worldPos, float panSpeed);

void ZoomToScreen(Vector2 screenPos, float zoomFactor);
void ZoomToWorld(Vector2 worldPos, float zoomFactor);

void ZoomCamera(float zoomFactor);
void SetCameraZoom(float zoom);

#endif	// GAME_CAMERA_H
