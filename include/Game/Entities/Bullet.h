#ifndef BULLET_H
#define BULLET_H

#include "ColorState.h"
#include "Common.h"

#define BULLET_RADIUS (WORLD_SCALE / 4)

typedef struct Bullet {
	Vector2 pos;
	float speed;
	uint16_t dir;
	ColorState colors;
} Bullet;

Bullet CreateBullet(Vector2 pos, float angle, float speed);

void DisplayBullet(const Bullet* bullet);
void UpdateBullet(Bullet* bullet);

#endif	// BULLET_H
