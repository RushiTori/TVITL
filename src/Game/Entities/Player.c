#include "Player.h"

#define DEFAULT_PLAYER_SIZES (Vector2){.x = (float)WORLD_SCALE, .y = (float)WORLD_SCALE};

Player CreatePlayer(Vector2 pos_) {
	Player player = {0};
	player.sizes = DEFAULT_PLAYER_SIZES;
	player.pos = pos_;
	return player;
}

void DisplayPlayer(const Player* player) {
	// WIP
	Rectangle playerSource = (Rectangle){
		.x = player->colors * 32,
		.y = 0,
		.width = 32,
		.height = 32,
	};

	const Rectangle playerBody = (Rectangle){
		.x = player->pos.x,
		.y = player->pos.y,
		.width = player->sizes.x,
		.height = player->sizes.y,
	};

	DrawTexturePro(EntitySheet, playerSource, playerBody, (Vector2){0}, 0, WHITE);
}

void UpdatePlayerInputs(Player* player) {
	if (IsKeyPressed(KEY_KP_1)) player->colors = InverseColorStateR(player->colors);
	if (IsKeyPressed(KEY_KP_2)) player->colors = InverseColorStateG(player->colors);
	if (IsKeyPressed(KEY_KP_3)) player->colors = InverseColorStateB(player->colors);

	player->vel = Vector2Zero();
	if (IsKeyDown(KEY_LEFT)) player->vel.x = -1;
	if (IsKeyDown(KEY_RIGHT)) player->vel.x = 1;

	if (IsKeyDown(KEY_UP)) player->vel.y = -1;
	if (IsKeyDown(KEY_DOWN)) player->vel.y = 1;

	player->vel = Vector2Normalize(player->vel);

	player->vel = Vector2Scale(player->vel, WORLD_SCALE);
	player->vel = Vector2Scale(player->vel, PLAYER_SPEED);
	player->vel = Vector2Scale(player->vel, (DELTA_TIME / COLLISIONS_STEPS));
}

void UpdatePlayerStates(Player* player) {
	// WIP
	player->pos = Vector2Add(player->pos, player->vel);
}

void CollidePlayerToZone(Player* restrict player, ZoneGrid* restrict grid, uint32_t zX, uint32_t zY) {
	Rectangle zoneBody = (Rectangle){
		.x = zX * WORLD_SCALE,
		.y = zY * WORLD_SCALE,
		.width = WORLD_SCALE,
		.height = WORLD_SCALE,
	};

	Rectangle playerBody = (Rectangle){
		.x = player->pos.x,
		.y = player->pos.y,
		.width = player->sizes.x,
		.height = player->sizes.y,
	};

	if (!CheckCollisionRecs(playerBody, zoneBody)) return;
	SetPlayerCollidedZone(grid, zX, zY);
	if (!IsZoneTypeSolid(GetZoneType(grid, zX, zY))) return;

	Rectangle displacement = GetCollisionRec(playerBody, zoneBody);
	if (displacement.width <= displacement.height) {
		if (player->pos.x > zoneBody.x) {
			player->pos.x += displacement.width;
		} else {
			player->pos.x -= displacement.width;
		}
	} else {
		if (player->pos.y > zoneBody.y) {
			player->pos.y += displacement.height;
		} else {
			player->pos.y -= displacement.height;
		}
	}
}

void CollidePlayerToZoneGrid(Player* restrict player, ZoneGrid* restrict grid, bool reverseChecks) {
	// WIP
	int32_t playerX = player->pos.x / WORLD_SCALE;
	int32_t playerY = player->pos.y / WORLD_SCALE;
	int32_t playerW = max(1, player->sizes.x / WORLD_SCALE);
	int32_t playerH = max(1, player->sizes.y / WORLD_SCALE);

	if (reverseChecks) {
		for (int32_t j = playerH; j >= -1; j--) {
			int32_t zY = playerY + j;
			if (zY < 0 || zY >= (int32_t)grid->height) continue;

			for (int32_t i = playerW; i >= -1; i--) {
				int32_t zX = playerX + i;
				if (zX < 0 || zX >= (int32_t)grid->width) continue;

				CollidePlayerToZone(player, grid, zX, zY);
			}
		}
	} else {
		for (int32_t j = -1; j <= playerH; j++) {
			int32_t zY = playerY + j;
			if (zY < 0 || zY >= (int32_t)grid->height) continue;

			for (int32_t i = -1; i <= playerW; i++) {
				int32_t zX = playerX + i;
				if (zX < 0 || zX >= (int32_t)grid->width) continue;

				CollidePlayerToZone(player, grid, zX, zY);
			}
		}
	}
}

void CollidePlayerToMover(Player* restrict player, const Mover* restrict mover) {
	// WIP
	Rectangle playerBody = (Rectangle){
		.x = player->pos.x,
		.y = player->pos.y,
		.width = player->sizes.x,
		.height = player->sizes.y,
	};

	Rectangle moverBody = (Rectangle){
		.x = mover->pos.x - (MOVER_SIZE / 2),
		.y = mover->pos.y - (MOVER_SIZE / 2),
		.width = MOVER_SIZE,
		.height = MOVER_SIZE,
	};

	if (CheckCollisionRecs(playerBody, moverBody)) player->isDead = true;
}

void CollidePlayerToOrbiter(Player* restrict player, const Orbiter* restrict orbiter) {
	Rectangle playerBody = (Rectangle){
		.x = player->pos.x,
		.y = player->pos.y,
		.width = player->sizes.x,
		.height = player->sizes.y,
	};

	// Vector2 centerOffset = (Vector2){.x = cosf(orbiter->angle), .y = sinf(orbiter->angle)};
	Vector2 centerOffset = (Vector2){.x = COS_TABLE[orbiter->angle], .y = SIN_TABLE[orbiter->angle]};
	centerOffset = Vector2Scale(centerOffset, orbiter->offset);
	Vector2 orbiterCenter = Vector2Add(orbiter->pos, centerOffset);

	if (CheckCollisionCircleRec(orbiterCenter, ORBITER_RADIUS, playerBody)) player->isDead = true;
}

void CollidePlayerToMovorbiter(Player* restrict player, const Movorbiter* restrict movorbiter) {
	if (movorbiter->useMover) CollidePlayerToMover(player, &movorbiter->mover);

	for (uint32_t i = 0; i < movorbiter->orbiters.size; i++) {
		CollidePlayerToOrbiter(player, movorbiter->orbiters.data + i);
	}
}

void CollidePlayerToShooter([[maybe_unused]] Player* restrict player,
							[[maybe_unused]] const Shooter* restrict shooter) {
	// WIP
}

void CollidePlayerToBullet(Player* restrict player, Bullet* restrict bullet) {
	Rectangle playerBody = (Rectangle){
		.x = player->pos.x,
		.y = player->pos.y,
		.width = player->sizes.x,
		.height = player->sizes.y,
	};

	if (CheckCollisionCircleRec(bullet->pos, BULLET_RADIUS, playerBody)) player->isDead = true;
}
