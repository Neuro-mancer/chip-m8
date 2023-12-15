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
#define TIMER_SPEED 60 // HZ
#define TIMER_TARGET_TIME (1000 / TIMER_SPEED) // in milliseconds
#define EMULATION_SPEED_MAX 4000 // HZ
#define EMULATION_SPEED_MIN 100 // HZ

struct Hardware 
{
	uint8_t *RAM;
	uint8_t V[NUM_REGISTERS]; // general purpose registers
	uint16_t PC; 
	uint16_t I; // index register for pointing to mem locations

	struct Stack
	{
		uint16_t items[STACK_SIZE];
		int8_t SP;
	} Stack;

	struct Timers
	{
		uint8_t delay;
		uint8_t sound;
		uint64_t lastFrameTime; // stores time elasped since last cycle
	} Timers;

	struct DisplayColors // the rgb values of the two colors on display
	{
		uint8_t background[3];
		uint8_t foreground[3];
	} DisplayColors;

	bool displayBuffer[DISPLAY_HEIGHT][DISPLAY_WIDTH];
	bool keyBuffer[NUM_KEYS];

	enum {EXECUTE, PAUSE, QUIT} STATE;
	bool debug;
	uint16_t currentInstruction;
	uint16_t emulationSpeed;
};

void decode(struct Hardware *chip8);

void fetch(struct Hardware *chip8);

void stackPush(uint16_t item, struct Hardware *chip8);

uint16_t stackPop(struct Hardware *chip8);

void initHardwareValues(struct Hardware *chip8);

bool loadROMIntoMem(char *fileName, struct Hardware *chip8);

void cycleTiming(struct Hardware *chip8);

void updateTimers(struct Hardware *chip8);

#endif // HARDWARE_H
