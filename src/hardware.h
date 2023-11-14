#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define RAM_SIZE 4096 
#define STACK_SIZE 16
#define NUM_REGISTERS 16
#define NUM_KEYS 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define PC_START 0x200
#define STACK_START -1
#define DELAY_TIMER_START 0
#define SOUND_TIMER_START 0
#define CYCLE_TIMER_START 0
#define MAX_ROM_SIZE 3584
#define CPU_CLOCK_SPEED 500 // HZ
#define CLOCK_TARGET_TIME (1000 / CPU_CLOCK_SPEED) // in milliseconds
#define TIMER_SPEED 60 // HZ
#define TIMER_TARGET_TIME (1000 / TIMER_SPEED) // in milliseconds

struct Hardware 
{
	uint8_t *RAM;
	uint8_t V[NUM_REGISTERS]; // general purpose registers
	uint16_t PC; 
	uint16_t I; // index register for pointing to mem locations

	struct Stack
	{
		uint16_t items[STACK_SIZE];
		uint8_t SP;
	} Stack;

	struct Timers
	{
		uint8_t delay;
		uint8_t sound;
		uint32_t lastCycleTime; // stores time elasped since last cycle
		uint32_t lastDelayTimerWrite;
		uint32_t lastSoundTimerWrite;
	} Timers;

	bool displayBuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH];
	bool keyBuffer[NUM_KEYS];
};

void decode(uint16_t opcode, struct Hardware *chip8);

uint16_t fetch(struct Hardware *chip8);

void stackPush(uint16_t item, struct Hardware *chip8);

uint16_t stackPop(struct Hardware *chip8);

void initHardwareValues(struct Hardware *chip8);

bool loadROMIntoMem(char *fileName, struct Hardware *chip8);

void cycleTiming(struct Hardware *chip8);

void updateTimers(struct Hardware *chip8);

#endif // HARDWARE_H
