#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "font.h"
#include "hardware.h"
#include "graphics.h"
#include "input.h"

#define PAUSE_IDLE_TIME 16

int main(int argc, char **argv)
{
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
			printf("Error: Unable to read rom, stopping program.\n");
			return -1;
		}
		else
		{
			graphicsInit();
			clearScreen();
			loadSound();
		
			chip8.STATE = EXECUTE;
			printf("CHIP-M8 Successfully loaded!");
			playSound();

			while(chip8.STATE != QUIT)
			{
				handleInput(&chip8);

				if(chip8.STATE == PAUSE)
				{
					SDL_Delay(PAUSE_IDLE_TIME);
					continue;
				}

				nextInstruction = fetch(&chip8);
				decode(nextInstruction, &chip8);
				cycleTiming(&chip8); // NOTE TO SELF: CHANGE CYCLE TIMING
				// TO USE PERFORMANCE COUNTER INSTEAD OF SDL GET TICKS
			}

			graphicsCleanup();
			free(chip8.RAM); 
		}

	}

	return 0;
}
