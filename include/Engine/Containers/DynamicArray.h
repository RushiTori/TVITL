#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "Bullet.h"
#include "Common.h"
#include "DynamicArrayTypes.h"
#include "Mover.h"
#include "Movorbiter.h"
#include "Orbiter.h"
#include "Shooter.h"
#include "ZoneGroup.h"
#include "ZoneTypeChange.h"

/*
common function :
realloc_container // main function used as wrapper around C realloc

unique functions :
create // self-explanatory, will invoke realloc_container
free // self-explanatory
reserve // make a DA have AT LEAST the inputed CAPACITY, can invoke realloc_container
shrink // make a DA have a capacity as low as possible, can invoke realloc_container
push_back // self-explanatory, can invoke realloc_container
remove // self-explanatory, can invoke realloc_container

utility function (macro func) :
map : takes a DA and a function ptr, invoke the func ptr on every element in the DA
*/

#define CreateDynamicArray(type)           \
	_Generic((typeof(type)){0},            \
		uint32_t: CreateIndexArray,        \
		Vector2: CreateVector2Array,           \
		Mover: CreateMoverArray,           \
		Orbiter: CreateOrbiterArray,       \
		Movorbiter: CreateMovorbiterArray, \
		Shooter: CreateShooterArray,       \
		Bullet: CreateBulletArray,         \
		ZoneGroup: CreateZoneGroupArray,   \
		ZoneTypeChange: CreateZoneTypeChangeArray)()

#define DynamicArray(type) typeof(CreateDynamicArray(type))

#define FreeDynamicArray(arr)                          \
	_Generic((typeof(*arr)){0},                        \
		DynamicArray(uint32_t): FreeIndexArray,        \
		DynamicArray(Vector2): FreeVector2Array,           \
		DynamicArray(Mover): FreeMoverArray,           \
		DynamicArray(Orbiter): FreeOrbiterArray,       \
		DynamicArray(Movorbiter): FreeMovorbiterArray, \
		DynamicArray(Shooter): FreeShooterArray,       \
		DynamicArray(Bullet): FreeBulletArray,         \
		DynamicArray(ZoneGroup): FreeZoneGroupArray,   \
		DynamicArray(ZoneTypeChange): FreeZoneTypeChangeArray)(arr)

#define ReserveDynamicArray(arr, capacity)                \
	_Generic((typeof(*arr)){0},                           \
		DynamicArray(uint32_t): ReserveIndexArray,        \
		DynamicArray(Vector2): ReserveVector2Array,           \
		DynamicArray(Mover): ReserveMoverArray,           \
		DynamicArray(Orbiter): ReserveOrbiterArray,       \
		DynamicArray(Movorbiter): ReserveMovorbiterArray, \
		DynamicArray(Shooter): ReserveShooterArray,       \
		DynamicArray(Bullet): ReserveBulletArray,         \
		DynamicArray(ZoneGroup): ReserveZoneGroupArray,   \
		DynamicArray(ZoneTypeChange): ReserveZoneTypeChangeArray)(arr, capacity)

#define ShrinkDynamicArray(arr)                          \
	_Generic((typeof(*arr)){0},                          \
		DynamicArray(uint32_t): ShrinkIndexArray,        \
		DynamicArray(Vector2): ShrinkVector2Array,           \
		DynamicArray(Mover): ShrinkMoverArray,           \
		DynamicArray(Orbiter): ShrinkOrbiterArray,       \
		DynamicArray(Movorbiter): ShrinkMovorbiterArray, \
		DynamicArray(Shooter): ShrinkShooterArray,       \
		DynamicArray(Bullet): ShrinkBulletArray,         \
		DynamicArray(ZoneGroup): ShrinkZoneGroupArray,   \
		DynamicArray(ZoneTypeChange): ShrinkZoneTypeChangeArray)(arr)

#define PushBackDynamicArray(arr, item)                    \
	_Generic((typeof(*arr)){0},                            \
		DynamicArray(uint32_t): PushBackIndexArray,        \
		DynamicArray(Vector2): PushBackVector2Array,           \
		DynamicArray(Mover): PushBackMoverArray,           \
		DynamicArray(Orbiter): PushBackOrbiterArray,       \
		DynamicArray(Movorbiter): PushBackMovorbiterArray, \
		DynamicArray(Shooter): PushBackShooterArray,       \
		DynamicArray(Bullet): PushBackBulletArray,         \
		DynamicArray(ZoneGroup): PushBackZoneGroupArray,   \
		DynamicArray(ZoneTypeChange): PushBackZoneTypeChangeArray)(arr, item)

#define RemoveDynamicArray(arr, idx)                     \
	_Generic((typeof(*arr)){0},                          \
		DynamicArray(uint32_t): RemoveIndexArray,        \
		DynamicArray(Vector2): RemoveVector2Array,           \
		DynamicArray(Mover): RemoveMoverArray,           \
		DynamicArray(Orbiter): RemoveOrbiterArray,       \
		DynamicArray(Movorbiter): RemoveMovorbiterArray, \
		DynamicArray(Shooter): RemoveShooterArray,       \
		DynamicArray(Bullet): RemoveBulletArray,         \
		DynamicArray(ZoneGroup): RemoveZoneGroupArray,   \
		DynamicArray(ZoneTypeChange): RemoveZoneTypeChangeArray)(arr, idx)

#define MapDynamicArray(arr, func)               \
	for (uint32_t i = 0; i < (arr)->size; i++) { \
		func((arr)->data + i);                   \
	}

#endif	// DYNAMIC_ARRAY_H
