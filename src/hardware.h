#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdbool.h>

#define RAM_SIZE 4096 // in bytes
#define STACK_SIZE 16
#define NUM_REGISTERS 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define PC_START 0x200
#define STACK_START -1
#define DELAY_TIMER_START 255
#define SOUND_TIMER_START 255
#define MAX_ROM_SIZE 3584 // in bytes

struct Hardware 
{
	uint8_t *RAM;
	uint8_t V[NUM_REGISTERS]; // general purpose registers
	uint16_t PC; 
	uint16_t I; // index register for pointing to mem locations

	struct Stack
	{
		uint16_t items[STACK_SIZE];
		int SP;
	} Stack;

	struct Timers
	{
		uint8_t delay;
		uint8_t sound;
	} Timers;

	bool displayBuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH];
};

void decode(uint16_t opcode, struct Hardware *chip8);

uint16_t fetch(struct Hardware *chip8);

void stackPush(uint16_t item, struct Hardware *chip8);

uint16_t stackPop(struct Hardware *chip8);

void initReg(struct Hardware *chip8);

bool loadROMIntoMem(char *fileName, struct Hardware *chip8);

#endif // HARDWARE_H
