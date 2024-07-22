#ifndef SHOOTER_H
#define SHOOTER_H

#include "Bullet.h"
#include "ColorState.h"
#include "Common.h"
#include "EntityPath.h"

typedef struct Level Level;

#define SHOOTER_SIZE WORLD_SCALE

typedef struct Shooter {
	Vector2 pos;
	EntityPath* pathRef;

	float angle;
	float angleStep;

	float fireRate;
	float fireTimer;
	float bulletSpeed;

	ColorState colors;
} Shooter;

Shooter CreateShooter(Vector2 pos, float baseAngle, float angleSteps, uint32_t fireRate, float bulletSpeed);

void SetShooterpath(Shooter* shooter, EntityPath* path);

void DisplayShooter(const Shooter* shooter);
void UpdateShooter(Shooter* shooter, Level* level);

#endif	// SHOOTER_H
