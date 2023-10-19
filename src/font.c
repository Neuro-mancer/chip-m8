#include "font.h"
#include <stdint.h>

void loadFontIntoMem(uint8_t *RAM)
{
	uint16_t fontArraySize = FONT_END_PTR - FONT_START_PTR;
	uint16_t byte = FONT_START_PTR;
	uint8_t fontArray[] = {FONT_VALUES};

	for(uint16_t i = 0; i < fontArraySize; i++)
	{
		RAM[byte] = fontArray[i];
		byte++;
	}
}
