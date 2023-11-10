#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "font.h"
#include "hardware.h"
#include "graphics.h"
#include "input.h"

int main(int argc, char **argv)
{
	bool quit = false;
	int nextInstruction;
	struct Hardware chip8;

	// init 4KB chip-8 memory and load fonts into memory
	uint8_t *RAM = (uint8_t *)malloc(RAM_SIZE * sizeof(uint8_t));
	if(RAM == NULL)
	{
		printf("Failed to allocate RAM memory to heap!\n");
	}
	else
	{
		chip8.RAM = RAM;
		initHardwareValues(&chip8);
		loadFontIntoMem(chip8.RAM);
		if(!loadROMIntoMem(argv[1], &chip8))
		{
			printf("Failed to load ROM into memory!\n");
		}
		else
		{
			graphicsInit();
			clearScreen();

			while(!quit)
			{
				quit = handleInput(&chip8); // look into making an enum which holds state of emulator?
				nextInstruction = fetch(&chip8);
				decode(nextInstruction, &chip8);
				cycleTiming(&chip8); // perhaps look into ways to speed up execution
				// updateTimers(&chip8); // order of this wrong?
			}

			graphicsCleanup();
			free(chip8.RAM); 
		}

	}
	return 0;
}
