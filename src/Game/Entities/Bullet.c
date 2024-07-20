#include "Bullet.h"

Bullet CreateBullet(Vector2 pos, float angle, float speed) {
	Bullet bullet = {0};
	bullet.pos = pos;
	bullet.dir = (angle / (PI * 2)) * UINT16_MAX;// WIP
	bullet.speed = speed * WORLD_SCALE;
	return bullet;
}

void DisplayBullet(const Bullet* bullet) {
	// WIP
	DrawCircleV(bullet->pos, BULLET_RADIUS, BLUE);
}

void UpdateBullet(Bullet* bullet) {
	// bullet->pos.x += cosf(bullet->dir) * bullet->speed * DELTA_TIME;
	// bullet->pos.y += sinf(bullet->dir) * bullet->speed * DELTA_TIME;
	bullet->pos.x += COS_TABLE[bullet->dir] * bullet->speed * DELTA_TIME;
	bullet->pos.y += SIN_TABLE[bullet->dir] * bullet->speed * DELTA_TIME;
}
