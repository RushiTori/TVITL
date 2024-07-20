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
	Color shooterCol;
	shooterCol.r = shooter->hasRed * 0xFF;
	shooterCol.g = shooter->hasGreen * 0xFF;
	shooterCol.b = shooter->hasBlue * 0xFF;
	shooterCol.a = 0xFF;

	DrawRectangleRec(
		(Rectangle){
			.x = shooter->pos.x - (SHOOTER_SIZE / 2),
			.y = shooter->pos.y - (SHOOTER_SIZE / 2),
			.width = SHOOTER_SIZE,
			.height = SHOOTER_SIZE,
		},
		shooterCol);
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
