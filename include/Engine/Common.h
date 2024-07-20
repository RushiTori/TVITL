#ifndef COMMON_H
#define COMMON_H

#include <inttypes.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leak_detector_c.h"

#ifdef max
#undef max
#endif	// max
#define max(a, b)                  \
	({                             \
		typeof(a) tmpA = (a);      \
		typeof(b) tmpB = (b);      \
		tmpA > tmpB ? tmpA : tmpB; \
	})

#ifdef min
#undef min
#endif	// min
#define min(a, b)                  \
	({                             \
		typeof(a) tmpA = (a);      \
		typeof(b) tmpB = (b);      \
		tmpA < tmpB ? tmpA : tmpB; \
	})

#define SwapValues(a, b)   \
	{                      \
		typeof(a) tmp = a; \
		a = b;             \
		b = tmp;           \
	}

#define GetIndex1D(x, y, w) ((x) + (y) * (w))

#define WORLD_SCALE 32
#define DELTA_TIME GetFrameTime()
#define COLLISIONS_STEPS 2

extern uint64_t frameCount;

extern float SIN_TABLE[UINT16_MAX + 1];
extern float COS_TABLE[UINT16_MAX + 1];

void InitSinCosTable();

#endif	// COMMON_H
