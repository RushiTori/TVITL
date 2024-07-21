#include "Bullet.h"
#include "Assets.h"

Bullet CreateBullet(Vector2 pos, float angle, float speed) {
	Bullet bullet = {0};
	bullet.pos = pos;
	bullet.dir = (angle / (PI * 2)) * UINT16_MAX;// WIP
	bullet.speed = speed * WORLD_SCALE;
	return bullet;
}

void DisplayBullet(const Bullet* bullet) {
	// WIP
	
	static const Rectangle bulletSource = (Rectangle){
		.x = 0,
		.y = 64,
		.width = 32,
		.height = 32,
	};

	const Rectangle bulletBody = (Rectangle){
		.x = bullet->pos.x - (BULLET_RADIUS / 2),
		.y = bullet->pos.y - (BULLET_RADIUS / 2),
		.width = BULLET_RADIUS,
		.height = BULLET_RADIUS,
	};
	
	DrawTexturePro(EntitySheet, bulletSource, bulletBody, (Vector2){0}, 0, WHITE);
}

void UpdateBullet(Bullet* bullet) {
	// bullet->pos.x += cosf(bullet->dir) * bullet->speed * DELTA_TIME;
	// bullet->pos.y += sinf(bullet->dir) * bullet->speed * DELTA_TIME;
	bullet->pos.x += COS_TABLE[bullet->dir] * bullet->speed * DELTA_TIME;
	bullet->pos.y += SIN_TABLE[bullet->dir] * bullet->speed * DELTA_TIME;
}
