#ifndef PLAYER_H
#define PLAYER_H

#include "Bullet.h"
#include "Mover.h"
#include "Movorbiter.h"
#include "Orbiter.h"
#include "Shooter.h"
#include "ZoneGrid.h"

#define PLAYER_SPEED 6

typedef struct Player {
	Vector2 pos;
	Vector2 vel;
	Vector2 sizes;
	bool hasRed;
	bool hasGreen;
	bool hasBlue;
	bool isDead;
} Player;

Player CreatePlayer(Vector2 pos_);

void DisplayPlayer(const Player* player);

void UpdatePlayerInputs(Player* player);
void UpdatePlayerStates(Player* player);

void CollidePlayerToZoneGrid(Player* restrict player, ZoneGrid* restrict grid, bool reverseChecks);

void CollidePlayerToMover(Player* restrict player, const Mover* restrict mover);
void CollidePlayerToOrbiter(Player* restrict player, const Orbiter* restrict orbiter);
void CollidePlayerToMovorbiter(Player* restrict player, const Movorbiter* restrict movorbiter);
void CollidePlayerToShooter(Player* restrict player, const Shooter* restrict shooter);
void CollidePlayerToBullet(Player* restrict player, Bullet* restrict bullet);

#endif	// PLAYER_H
