#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

#define SHEET_CELL_SIZE 32
#define SHEET_WIDTH 16
#define SHEET_UV_SCALE 1.0 / SHEET_WIDTH

#define ZONE_EMPTY        46 // '.'
#define ZONE_WALL         35 // '#'
#define ZONE_FAKEWALL     48 // '0'
#define ZONE_DEATHWALL   120 // 'x'
#define ZONE_COLORWALL    99 // 'c'
#define ZONE_TELEPORTER  111 // 'o'
#define ZONE_CHECKPOINT   42 // '*'
#define ZONE_ENDPOINT     43 // '+'
#define ZONE_KEYWALL      61 // '='
#define ZONE_TRIGGERPOINT 37 // '%'

uniform int gridWidth;
uniform int gridHeight;
uniform sampler2D zoneNeighs;

uniform sampler2D wallSheet;
uniform sampler2D fakewallSheet;
uniform sampler2D deathwallSheet;
uniform sampler2D colorwallSheet;
uniform sampler2D teleporterSheet;
uniform sampler2D checkpointSheet;
uniform sampler2D endpointSheet;
uniform sampler2D keywallSheet;
uniform sampler2D triggerpointSheet;

void main() {
	vec2 zoneIdx = (floor(fragTexCoord) + 0.5) / vec2(gridWidth, gridHeight);

	int neighs = int(texture(zoneNeighs, zoneIdx).r * 255);
	vec2 sheetOffset = vec2((neighs % SHEET_WIDTH), neighs / SHEET_WIDTH) * SHEET_UV_SCALE;

	vec2 sheetUV = fract(fragTexCoord) * SHEET_UV_SCALE;

	int type = int(texture(texture0, zoneIdx).r * 255);
	/*switch (type) {
		default:
		case ZONE_EMPTY:
			finalColor = vec4(0.0);
			break;

		case ZONE_WALL:
			finalColor = texture(wallSheet, sheetOffset + sheetUV);
			break;

		case ZONE_FAKEWALL:
			finalColor = texture(fakewallSheet, sheetOffset + sheetUV);
			break;

		case ZONE_DEATHWALL:
			finalColor = texture(deathwallSheet, sheetOffset + sheetUV);
			break;

		case ZONE_COLORWALL:
			finalColor = texture(colorwallSheet, sheetOffset + sheetUV);
			break;

		case ZONE_TELEPORTER:
			finalColor = texture(teleporterSheet, sheetOffset + sheetUV);
			break;

		case ZONE_CHECKPOINT:
			finalColor = texture(checkpointSheet, sheetOffset + sheetUV);
			break;

		case ZONE_ENDPOINT:
			finalColor = texture(endpointSheet, sheetOffset + sheetUV);
			break;

		case ZONE_KEYWALL:
			finalColor = texture(keywallSheet, sheetOffset + sheetUV);
			break;

		case ZONE_TRIGGERPOINT:
			finalColor = texture(triggerpointSheet, sheetOffset + sheetUV);
			break;
	}*/

	if (type == ZONE_WALL) {
		finalColor = texture(wallSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_FAKEWALL) {
		finalColor = texture(fakewallSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_DEATHWALL) {
		finalColor = texture(deathwallSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_COLORWALL) {
		finalColor = texture(colorwallSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_TELEPORTER) {
		finalColor = texture(teleporterSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_CHECKPOINT) {
		finalColor = texture(checkpointSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_ENDPOINT) {
		finalColor = texture(endpointSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_KEYWALL) {
		finalColor = texture(keywallSheet, sheetOffset + sheetUV);
	} else if (type == ZONE_TRIGGERPOINT) {
		finalColor = texture(triggerpointSheet, sheetOffset + sheetUV);
	} else {
		finalColor = vec4(1, 1, 1, 1);
	}

	finalColor *= fragColor;
}
