#ifndef BULLET_H
#define BULLET_H

#include "Common.h"

#define BULLET_RADIUS (WORLD_SCALE / 4)

typedef struct Bullet {
	Vector2 pos;
	uint16_t dir;
	float speed;
} Bullet;

Bullet CreateBullet(Vector2 pos, float angle, float speed);

void DisplayBullet(const Bullet* bullet);
void UpdateBullet(Bullet* bullet);

#endif	// BULLET_H
