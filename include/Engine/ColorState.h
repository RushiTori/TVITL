#ifndef COLOR_STATE_H
#define COLOR_STATE_H

#include "Common.h"

typedef uint8_t ColorState;

#define ColorState_R_MASK 0b001
#define ColorState_G_MASK 0b010
#define ColorState_B_MASK 0b100

#define ColorState_R_SHIFT 0
#define ColorState_G_SHIFT 1
#define ColorState_B_SHIFT 2

#define CreateColorState(r, g, b) \
	((r) << ColorState_R_SHIFT) | ((g) << ColorState_G_SHIFT) | ((b) << ColorState_B_SHIFT)

#define GetColorStateR(state) (((state) & ColorState_R_MASK) >> ColorState_R_SHIFT)
#define SetColorStateR(state) ((state) | ColorState_R_MASK)
#define ClearColorStateR(state) ((state) & ((~ColorState_R_MASK) & 0b111))
#define InverseColorStateR(state) ((state) ^ ColorState_R_MASK)

#define GetColorStateG(state) (((state) & ColorState_G_MASK) >> ColorState_G_SHIFT)
#define SetColorStateG(state) ((state) | ColorState_G_MASK)
#define ClearColorStateG(state) ((state) & ((~ColorState_G_MASK) & 0b111))
#define InverseColorStateG(state) ((state) ^ ColorState_G_MASK)

#define GetColorStateB(state) (((state) & ColorState_B_MASK) >> ColorState_B_SHIFT)
#define SetColorStateB(state) ((state) | ColorState_B_MASK)
#define ClearColorStateB(state) ((state) & ((~ColorState_B_MASK) & 0b111))
#define InverseColorStateB(state) ((state) ^ ColorState_B_MASK)

#define CS_BLACK CreateColorState(false, false, false)
#define CS_BLUE CreateColorState(false, false, true)
#define CS_GREEN CreateColorState(false, true, false)
#define CS_CYAN CreateColorState(false, true, true)
#define CS_RED CreateColorState(true, false, false)
#define CS_MAGENTA CreateColorState(true, false, true)
#define CS_YELLOW CreateColorState(true, true, false)
#define CS_WHITE CreateColorState(true, true, true)

typedef enum ColorStateSignal {
	CS_SIGNAL_LIVE = 0,
	CS_SIGNAL_DIE,
	CS_SIGNAL_IMMORTAL,
} ColorStateSignal;

#define GetColorStateSignal(stateA, stateB)       \
	({                                            \
		ColorState a = stateA;              \
		ColorState b = stateB;              \
                                                  \
		ColorStateSignal signal = CS_SIGNAL_LIVE; \
		if (b == CS_WHITE) {                      \
			signal = CS_SIGNAL_IMMORTAL;          \
		} else if (b == CS_BLACK || a != b) {     \
			signal = CS_SIGNAL_DIE;               \
		}                                         \
                                                  \
		signal;                                   \
	})

#define ColorStateAsColor(state)                     \
	({                                               \
		ColorState tmp = state;                      \
		Color col;                                   \
		col.r = (GetColorStateR(tmp)) ? 0xFF : 0x32; \
		col.g = (GetColorStateG(tmp)) ? 0xFF : 0x32; \
		col.b = (GetColorStateB(tmp)) ? 0xFF : 0x32; \
		col.a = 0xFF;                                \
		col;                                         \
	})

#endif	// COLOR_STATE_H
