#ifndef FIXED16_H
#define FIXED16_H

#include "Common.h"

// u11.5
typedef uint16_t fixed16;

#define FIXED16_INTEGER_BITWIDTH 11
#define FIXED16_INTEGER_MASK 0b1111111111100000

#define FIXED16_FRACTIONAL_BITWIDTH 5
#define FIXED16_FRACTIONAL_MASK 0b0000000000011111

#define FIXED16_BITWIDTH_MASK 0b1111111111111111

#define FIXED16_ONE (1 << FIXED16_FRACTIONAL_BITWIDTH)
#define FIXED16_HALF (FIXED16_ONE >> 1)
#define FIXED16_MAX FIXED16_BITWIDTH_MASK

#define UINT_TO_FIXED16(a) (((a) << FIXED16_FRACTIONAL_BITWIDTH) & FIXED16_BITWIDTH_MASK)
#define FIXED16_TO_UINT(a) ((a) >> FIXED16_FRACTIONAL_BITWIDTH)

#define FLOAT_TO_FIXED16(a) ((fixed16)roundf((a) * FIXED16_ONE))
#define FIXED16_TO_FLOAT(a) ((float)(a) / FIXED16_ONE)

#define FIXED16_FRACT(a) ((a) & FIXED16_FRACTIONAL_MASK)
#define FIXED16_FLOOR(a) ((a) & FIXED16_INTEGER_MASK)

#define FIXED16_CEIL(a)                         \
	({                                          \
		fixed16 tmp = a;                        \
		fixed16 intPart = FIXED16_FLOOR(tmp);   \
		fixed16 fractPart = FIXED16_FRACT(tmp); \
		fixed16 result = intPart;               \
		if (fractPart) result += FIXED16_ONE;   \
		result;                                 \
	})

#define FIXED16_ROUND(a)                                      \
	({                                                        \
		fixed16 tmp = a;                                      \
		fixed16 intPart = FIXED16_FLOOR(tmp);                 \
		fixed16 fractPart = FIXED16_FRACT(tmp);               \
		fixed16 result = intPart;                             \
		if (fractPart >= FIXED16_HALF) result += FIXED16_ONE; \
		result;                                               \
	})

#define FIXED16_MUL(a, b) (((((uint32_t)(a)) * (b)) + FIXED16_HALF) >> FIXED16_FRACTIONAL_BITWIDTH)

#endif	// FIXED16_H
