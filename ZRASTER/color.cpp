#include "color.h"

uint32_t getcolor(float lum)
{
	uint32_t col;

	int pixel_bw = (int)(25.0f * lum);

	switch (pixel_bw) {
	case 0:  col = 0x00000000; break;
	case 1:  col = 0x080808af; break;
	case 2:  col = 0x0f0f0faf; break;
	case 3:  col = 0x171717ff; break;
	case 4:  col = 0x1f1f1fcf; break;
	case 5:  col = 0x262626ff; break;
	case 6:  col = 0x2e2e2eff; break;
	case 7:  col = 0x363636cf; break;
	case 8:  col = 0x3d3d3ddf; break;
	case 9:  col = 0x454545af; break;
	case 10: col = 0x4d4d4dbf; break;
	case 11: col = 0x575757cf; break;
	case 12: col = 0x595959df; break;
	case 13: col = 0x5e5e5edf; break;
	case 14: col = 0x666666ff; break;
	case 15: col = 0x6b6b6bff; break;
	case 16: col = 0x787878df; break;
	case 17: col = 0xa1a1a1af; break;
	case 18: col = 0xa1a1a1bf; break;
	case 19: col = 0xbababaff; break;
	case 20: col = 0xcdcdcdff; break;
	case 21: col = 0xcdcdcdff; break;
	case 22: col = 0xfafafaff; break;
	case 23: col = 0xfafafaff; break;
	case 24: col = 0xfcfcfcff; break;
	case 25: col = 0xffffffff; break;
	default: col = 0x00000000; break;
	}

	return col;
}