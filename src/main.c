#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h> // remove when done with sleep function???
#include "font.h"
#include "hardware.h"
#include "graphics.h"
#include "input.h"

int main(int argc, char **argv)
{
	// bool quit = false;
	int nextInstruction;
	// initialize our chip-8 virtual hardware object
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
		initReg(&chip8);
		loadFontIntoMem(chip8.RAM);
		if(!loadROMIntoMem(argv[1], &chip8))
		{
			printf("Failed to load ROM into memory!\n");
		}
		else
		{
			graphicsInit();
			clearScreen();
			int cycle = 0;

			while(cycle < 20)
			{
				// quit = handleInput();
				handleInput();
				nextInstruction = fetch(&chip8);
				decode(nextInstruction, &chip8);
				cycle++;
			}
			sleep(10);

			graphicsCleanup();
			free(chip8.RAM); 
		}

	}
	return 0;
}
