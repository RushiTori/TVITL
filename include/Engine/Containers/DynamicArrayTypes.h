#ifndef DYNAMIC_ARRAY_TYPES_H
#define DYNAMIC_ARRAY_TYPES_H

#include "Common.h"

#define DECLARE_DYNAMIC_ARRAY_TYPE(type, structName) \
	typedef struct structName {                      \
		type* data;                                  \
		uint32_t capacity;                           \
		uint32_t size;                               \
	} structName;

#define DECLARE_DYNAMIC_ARRAY_FUNCTIONS(type, structName)         \
	structName Create##structName();                              \
	void Free##structName(structName* arr);                       \
                                                                  \
	void Reserve##structName(structName* arr, uint32_t capacity); \
	void Shrink##structName(structName* arr);                     \
                                                                  \
	void PushBack##structName(structName* arr, type item);        \
	void Remove##structName(structName* arr, uint32_t idx);

DECLARE_DYNAMIC_ARRAY_TYPE(uint32_t, IndexArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(uint32_t, IndexArray)

DECLARE_DYNAMIC_ARRAY_TYPE(Vector2, Vector2Array)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(Vector2, Vector2Array)

typedef struct Mover Mover;
DECLARE_DYNAMIC_ARRAY_TYPE(Mover, MoverArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(Mover, MoverArray)

typedef struct Orbiter Orbiter;
DECLARE_DYNAMIC_ARRAY_TYPE(Orbiter, OrbiterArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(Orbiter, OrbiterArray)

typedef struct Movorbiter Movorbiter;
DECLARE_DYNAMIC_ARRAY_TYPE(Movorbiter, MovorbiterArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(Movorbiter, MovorbiterArray)

typedef struct Shooter Shooter;
DECLARE_DYNAMIC_ARRAY_TYPE(Shooter, ShooterArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(Shooter, ShooterArray)

typedef struct ZoneGroup ZoneGroup;
DECLARE_DYNAMIC_ARRAY_TYPE(ZoneGroup, ZoneGroupArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(ZoneGroup, ZoneGroupArray)

typedef struct Bullet Bullet;
DECLARE_DYNAMIC_ARRAY_TYPE(Bullet, BulletArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(Bullet, BulletArray)

typedef struct ZoneTypeChange ZoneTypeChange;
DECLARE_DYNAMIC_ARRAY_TYPE(ZoneTypeChange, ZoneTypeChangeArray)
DECLARE_DYNAMIC_ARRAY_FUNCTIONS(ZoneTypeChange, ZoneTypeChangeArray)

#undef DECLARE_DYNAMIC_ARRAY_TYPE
#undef DECLARE_DYNAMIC_ARRAY_FUNCTIONS

#endif	// DYNAMIC_ARRAY_TYPES_H
