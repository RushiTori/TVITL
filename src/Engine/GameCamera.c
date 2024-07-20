#include "GameCamera.h"

Camera2D gameCamera = (Camera2D){
	.offset = (Vector2){0},
	.target = (Vector2){0},
	.rotation = 0,
	.zoom = 1,
};

void InitGameCamera() {
	gameCamera.offset.x = GetScreenWidth() / 2;
	gameCamera.offset.y = GetScreenHeight() / 2;
	gameCamera.target.x = GetScreenWidth() / 2;
	gameCamera.target.y = GetScreenHeight() / 2;
}

Rectangle GetWorldView() {
	Vector2 posA = GetScreenToGameWorld((Vector2){0});
	Vector2 posB = GetScreenToGameWorld((Vector2){
		.x = GetScreenWidth(),
		.y = GetScreenHeight(),
	});

	posB = Vector2Subtract(posB, posA);
	return (Rectangle){
		.x = posA.x,
		.y = posA.y,
		.width = posB.x,
		.height = posB.y,
	};
}

Vector2 GetScreenToGameWorld(Vector2 screenPos) { return GetScreenToWorld2D(screenPos, gameCamera); }

Vector2 GetScreenToGameTile(Vector2 screenPos) {
	Vector2 tilePos = Vector2Scale(GetScreenToGameWorld(screenPos), 1.0f / WORLD_SCALE);
	tilePos.x = floorf(tilePos.x);
	tilePos.y = floorf(tilePos.y);
	return tilePos;
}

Vector2 GetGameWorldToGameTile(Vector2 worldPos) { return Vector2Scale(worldPos, 1.0f / WORLD_SCALE); }

Vector2 GetGameWorldToScreen(Vector2 worldPos) { return GetWorldToScreen2D(worldPos, gameCamera); }

void SetCameraPosScreen(Vector2 screenPos) { SetCameraPosWorld(GetScreenToGameWorld(screenPos)); }

void SetCameraPosWorld(Vector2 worldPos) { gameCamera.target = worldPos; }

void MoveCameraByScreen(Vector2 vel) { MoveCameraByWorld(Vector2Scale(vel, 1.0f / gameCamera.zoom)); }

void MoveCameraByWorld(Vector2 vel) { gameCamera.target = Vector2Add(gameCamera.target, vel); }

void PanToScreen(Vector2 screenPos, float panSpeed) { PanToWorld(GetScreenToGameWorld(screenPos), panSpeed); }

void PanToWorld(Vector2 worldPos, float panSpeed) {
	Vector2 vel = Vector2Subtract(worldPos, gameCamera.target);
	float dist2 = Vector2LengthSqr(vel);
	if (dist2 == 0) return;
	vel = Vector2Normalize(vel);
	vel = Vector2Scale(vel, panSpeed * WORLD_SCALE * DELTA_TIME);
	float mag2 = Vector2LengthSqr(vel);

	if (mag2 > dist2) vel = Vector2Subtract(worldPos, gameCamera.target);
	MoveCameraByWorld(vel);
}

void ZoomToScreen(Vector2 screenPos, float zoomFactor) { ZoomToWorld(GetScreenToGameWorld(screenPos), zoomFactor); }

void ZoomToWorld(Vector2 worldPos, float zoomFactor) {
	Vector2 screenAnchor = GetGameWorldToScreen(worldPos);
	ZoomCamera(zoomFactor);

	Vector2 worldAnchor = GetScreenToGameWorld(screenAnchor);
	MoveCameraByWorld(Vector2Subtract(worldPos, worldAnchor));
}

void ZoomCamera(float zoomFactor) { gameCamera.zoom *= zoomFactor; }

void SetCameraZoom(float zoom) { gameCamera.zoom = zoom; }
