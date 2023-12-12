#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "font.h"
#include "hardware.h"
#include "graphics.h"
#include "input.h"

#define PAUSE_IDLE_TIME 16

void initNcursesDebug(int *maxY, int *maxX);
void initNcursesWin(int maxY, int maxX, WINDOW *stackWin, WINDOW *genRegWin, WINDOW *instructionWin);
void updateStackWin(WINDOW *stackWin, struct Hardware *chip8);
void updateRegWin(WINDOW *genRegWin, struct Hardware *chip8);
void updateInstructionWin(WINDOW *instructionWin, struct Hardware *chip8);

int main(int argc, char **argv)
{
	int success = 0;
	struct Hardware chip8;

	// ncurses vars
	int maxY, maxX;
	WINDOW *stackWin = NULL;
	WINDOW *genRegWin = NULL;
	WINDOW *instructionWin = NULL;


	// init 4KB chip-8 memory and load fonts into memory
	uint8_t *RAM = (uint8_t *)malloc(RAM_SIZE * sizeof(uint8_t));

	if(RAM == NULL)
	{
		printf("Failed to allocate RAM memory to heap!\n");
		success = -1;
	}
	else
	{
		chip8.RAM = RAM;
		initHardwareValues(&chip8);
		loadFontIntoMem(chip8.RAM);

		if(!loadROMIntoMem(argv[1], &chip8))
		{
			printf("Error: Unable to read rom, stopping program.\n");
			success = -1;
		}
		else
		{
			int optionIndex = 0;
			int r, g, b, R, G, B;
			bool argSuccess = true;

			while((optionIndex = getopt(argc, argv, "dr:g:b:R:G:B:")) != -1)
			{
				switch(optionIndex)
				{
					case 'd':
						chip8.debug = true;
						break;
					case 'r':
						r = atoi(optarg);
						chip8.DisplayColors.foreground[0] = r;
						break;
					case 'g':
						g = atoi(optarg);
						chip8.DisplayColors.foreground[1] = g;
						break;
					case 'b':
						b = atoi(optarg);
						chip8.DisplayColors.foreground[2] = b;
						break;
					case 'R':
						R = atoi(optarg);
						chip8.DisplayColors.background[0] = R;
						break;
					case 'G':
						G = atoi(optarg);
						chip8.DisplayColors.background[1] = G;
						break;
					case 'B':
						B = atoi(optarg);
						chip8.DisplayColors.background[2] = B;
						break;
					default:
						printf("Optional argument not recognized, aborting program.\n");
						argSuccess = false;
						break;
				}
			}

			if(argSuccess)
			{
				graphicsInit();
				clearScreen(&chip8);
				SDL_RenderPresent(renderer);
				loadSound();
			
				chip8.STATE = PAUSE;

				printf("CHIP-M8 Successfully loaded.\n");
				printf("Note: CHIP-M8 defaults to a paused state upon loading a rom, unpause/pause with the Escape button.\n");

				if(chip8.debug)
				{
					initNcursesDebug(&maxY, &maxX);
					stackWin = newwin((maxY / 2) - 2, (maxX / 2) - 2, 1, 1);
					genRegWin = newwin((maxY / 2) - 2, (maxX / 2) - 2, 1, (maxX / 2) + 1);
					instructionWin = newwin((maxY / 2), maxX - 2, maxY / 2, 1);

					initNcursesWin(maxY, maxX, stackWin, genRegWin, instructionWin);
					refresh();
					wrefresh(stackWin);
					wrefresh(genRegWin);
					wrefresh(instructionWin);
				}

				while(chip8.STATE != QUIT)
				{
					chip8.Timers.lastFrameTime = SDL_GetPerformanceCounter();

					if(chip8.debug)
					{
						updateStackWin(stackWin, &chip8);
						updateRegWin(genRegWin, &chip8);
						updateInstructionWin(instructionWin, &chip8);
					}

					handleInput(&chip8);

					if(chip8.STATE == PAUSE)
					{
						SDL_Delay(PAUSE_IDLE_TIME);
						continue;
					}

					for(int i = 0; i < chip8.emulationSpeed / 60; i++)
					{
						fetch(&chip8);
						decode(&chip8);
					}

					cycleTiming(&chip8);
				}

				graphicsCleanup();
				free(chip8.RAM); 
				endwin();
			}
			else
			{
				success = -1;
			}
		}

	}

	return success;
}

void initNcursesDebug(int *maxY, int *maxX)
{
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	getmaxyx(stdscr, *maxY, *maxX);
}

void initNcursesWin(int maxY, int maxX, WINDOW *stackWin, WINDOW *genRegWin, WINDOW *instructionWin)
{
	box(stdscr, 0, 0);
	mvprintw(0, (maxX / 2) - 13, "DEBUG INTERFACE");

	box(stackWin, 0, 0);
	mvwprintw(stackWin, 0, 0, "STACK:");

	box(genRegWin, 0, 0);
	mvwprintw(genRegWin, 0, 0, "GENERAL-PURPOSE REGISTERS:");

	box(instructionWin, 0, 0);
	mvwprintw(instructionWin, 0, 0, "INSTRUCTION AND EXECUTION:");
}

void updateStackWin(WINDOW *stackWin, struct Hardware *chip8)
{
	mvwprintw(stackWin, 2, 2, "Stack Pointer: 0x%01X", chip8->Stack.SP);
	for(int memLoc = 0; memLoc < STACK_SIZE; memLoc++)
	{
		mvwprintw(stackWin, 4 + memLoc, 2, "0x%01X: 0x%04X", memLoc, chip8->Stack.items[memLoc]);
	}
	wrefresh(stackWin);
}

void updateRegWin(WINDOW *genRegWin, struct Hardware *chip8)
{
	mvwprintw(genRegWin, 2, 2, "Flag Register 0xF: 0x%02X", chip8->V[0xF]);
	for(int memLoc = 0; memLoc < (NUM_REGISTERS - 1); memLoc++)
	{
		mvwprintw(genRegWin, 4 + memLoc, 2, "0x%01X: 0x%02X", memLoc, chip8->V[memLoc]);
	}
	mvwprintw(genRegWin, 20, 2, "Delay Timer Register: 0x%02X", chip8->Timers.sound);
	mvwprintw(genRegWin, 21, 2, "Sound Timer Register: 0x%02X", chip8->Timers.delay);
	mvwprintw(genRegWin, 22, 2, "Index Register: 0x%04X", chip8->I);
	wrefresh(genRegWin);
}

void updateInstructionWin(WINDOW *instructionWin, struct Hardware *chip8)
{
	mvwprintw(instructionWin, 2, 2, "Program Counter: 0x%04X", chip8->PC - 2);
	mvwprintw(instructionWin, 3, 2, "Current Instruction: 0x%04X", chip8->currentInstruction);
	mvwprintw(instructionWin, 4, 2, "Target Emulation Speed: ~%uHz", chip8->emulationSpeed);
	if(chip8->STATE == PAUSE)
	{
		mvwprintw(instructionWin, 5, 2, "Current Emulator State: Pause");
	}
	else 
	{
		mvwprintw(instructionWin, 5, 2, "Current Emulator State: Execute");
	}
	wrefresh(instructionWin);
}
