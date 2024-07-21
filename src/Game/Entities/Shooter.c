#include "Shooter.h"

#include "Level.h"

Shooter CreateShooter(Vector2 pos, float baseAngle, float angleStep, uint32_t fireRate, float bulletSpeed) {
	Shooter shooter = {0};

	shooter.pos = pos;

	shooter.angle = baseAngle;
	shooter.angleStep = angleStep;

	shooter.fireRate = 1.0f / max(1U, fireRate);
	shooter.bulletSpeed = bulletSpeed;

	return shooter;
}

void SetShooterpath(Shooter* shooter, EntityPath* path) {
	shooter->pathRef = path;
	SetEntityPathEntityRef(path, &shooter->pos);
}

void DisplayShooter(const Shooter* shooter) {
	//WIP
	static const Rectangle shooterSource = (Rectangle){
		.x = 0,
		.y = 32,
		.width = 32,
		.height = 32,
	};

	const Rectangle shooterBody = (Rectangle){
		.x = shooter->pos.x - (SHOOTER_SIZE / 2),
		.y = shooter->pos.y - (SHOOTER_SIZE / 2),
		.width = SHOOTER_SIZE,
		.height = SHOOTER_SIZE,
	};
	
	DrawTexturePro(EntitySheet, shooterSource, shooterBody, (Vector2){0.5, 0.5}, shooter->angle * RAD2DEG, WHITE);
}

void UpdateShooter(Shooter* shooter, Level* level) {
	shooter->fireTimer += DELTA_TIME;
	if (shooter->fireTimer >= shooter->fireRate) {
		shooter->fireTimer -= shooter->fireRate;
		PushBackDynamicArray(&level->bullets, CreateBullet(shooter->pos, shooter->angle, shooter->bulletSpeed));
		shooter->angle += shooter->angleStep;
	}
	UpdateEntityPath(shooter->pathRef);
}
