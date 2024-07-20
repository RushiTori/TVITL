#include "Common.h"

uint64_t frameCount = 0;

float SIN_TABLE[0x10000] = {0};
float COS_TABLE[0x10000] = {0};

void InitSinCosTable() {
	// WIP
	const float angleOffset = (PI * 2) / UINT16_MAX;

	float currAngle = 0;

	for (uint32_t i = 0; i <= UINT16_MAX; i++) {
		COS_TABLE[i] = cosf(currAngle);
		SIN_TABLE[i] = sinf(currAngle);
		currAngle += angleOffset;
	}
}
