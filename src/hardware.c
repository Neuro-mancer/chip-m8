#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hardware.h"
#include "instructions.h"
#include "graphics.h"

void stackPush(uint16_t item, struct Hardware *chip8)
{
	if(chip8->Stack.SP == STACK_SIZE)
	{
		printf("ERROR: CHIP-8 STACK OVERFLOW\n");
	}
	else
	{
		chip8->Stack.SP++;
		chip8->Stack.items[chip8->Stack.SP] = item;
	}
}

uint16_t stackPop(struct Hardware *chip8)
{
	uint16_t popped = 0;
	if(chip8->Stack.SP == -1)
	{
		printf("ERROR: CHIP-8 STACK UNDERFLOW\n");
	}
	else
	{
		popped = chip8->Stack.items[chip8->Stack.SP]; 
		chip8->Stack.SP--;
	}

	return popped;	
}

void initHardwareValues(struct Hardware *chip8)
{
	chip8->PC = PC_START;
	chip8->Stack.SP = STACK_START;
	chip8->Timers.delay = DELAY_TIMER_START;
	chip8->Timers.sound = SOUND_TIMER_START;
	chip8->Timers.lastCycleTime = CYCLE_TIMER_START;
	chip8->Timers.lastDelayTimerWrite = DELAY_TIMER_START;
	chip8->Timers.lastSoundTimerWrite = SOUND_TIMER_START;
	memset(chip8->displayBuffer, false, sizeof chip8->displayBuffer);
	memset(chip8->keyBuffer, false, sizeof chip8->keyBuffer);
}

uint16_t fetch(struct Hardware *chip8)
{
	// note: chip-8 opcodes are stored big endian 
	uint16_t opcodeByteOne, opcodeByteTwo;
	uint16_t opcode; // look into bitcodes for instructions, might be useful

	opcodeByteOne = chip8->RAM[chip8->PC]; 
	opcodeByteTwo = chip8->RAM[chip8->PC + 1];

	opcode = (opcodeByteOne << 8) | opcodeByteTwo;
	// printf("Opcode to be executed: 0x%04x | current instruction address: 0x%04x\n", opcode, chip8->PC);
	fflush(stdout);

	chip8->PC += 2;

	return opcode;
}

void decode(uint16_t opcode, struct Hardware *chip8)
{
	uint16_t nibbleMaskOne = opcode & 0xF000;
	uint16_t nibbleMaskTwo = opcode & 0x0F00;
	uint16_t nibbleMaskThree = opcode & 0x00F0;
	uint16_t nibbleMaskFour = opcode & 0x000F;
	uint16_t byteMaskTwo = opcode & 0x00FF;
	uint16_t addrMask = opcode & 0x0FFF;

	uint16_t digitTwo = nibbleMaskTwo >> 8; // for passing into instructions as arguments
	uint16_t digitThree = nibbleMaskThree >> 4;

	switch(nibbleMaskOne)
	{
		case 0x0000:
			switch(byteMaskTwo)
			{
				case 0x00E0:
					execClearDisplay(chip8);
					break;
				case 0x00EE:
					execRet(chip8);
					break;
				default:
					printf("Instruction 0x%04x not recognized!\n", opcode);
					break;
			}
			break;
		case 0x1000:
			execJump(addrMask, chip8);
			break;
		case 0x2000:
			execSubroutine(addrMask, chip8);
			break;
		case 0x3000:
			execSkipEqual(byteMaskTwo, digitTwo, chip8);
			break;
		case 0x4000:
			execSkipNotEqual(byteMaskTwo, digitTwo, chip8);
			break;
		case 0x5000:
			execSkipRegEqual(digitTwo, digitThree, chip8);
			break;
		case 0x6000:
			execSetReg(byteMaskTwo, digitTwo, chip8);
			break;
		case 0x7000:
			execAddValToReg(byteMaskTwo, digitTwo, chip8);
			break;
		case 0x8000:
			switch(nibbleMaskFour)
			{
				case 0x0000:
					execSetRegXToRegY(digitTwo, digitThree, chip8);
					break;
				case 0x0001:
					execOR(digitTwo, digitThree, chip8);
					break;
				case 0x0002:
					execAND(digitTwo, digitThree, chip8);
					break;
				case 0x0003:
					execXOR(digitTwo, digitThree, chip8);
					break;
				case 0x0004:
					execAdd(digitTwo, digitThree, chip8);
					break;
				case 0x0005:
					execSubtYFromX(digitTwo, digitThree, chip8);
					break;
				case 0x0006:
					execShiftRight(digitTwo, digitThree, chip8);
					break;
				case 0x0007:
					execSubtXFromY(digitTwo, digitThree, chip8);
					break;
				case 0x000E:
					execShiftLeft(digitTwo, digitThree, chip8);
					break;
				default:
					printf("Instruction 0x%04x not recognized!\n", opcode);
					break;
			}
			break;
		case 0x9000:
			execSkipRegNotEqual(digitTwo, digitThree, chip8);
			break;
		case 0xA000:
			execSetIndexReg(addrMask, chip8);
			break;
		case 0xB000:
			execJumpOffset(addrMask, chip8);
			break;
		case 0xC000:
			execGenRandom(byteMaskTwo, digitTwo, chip8);
			break;
		case 0xD000:
			execDraw(digitTwo, digitThree, nibbleMaskFour, chip8);
			break;
		case 0xE000:
			switch(byteMaskTwo)
			{
				case 0x009E:
					execSkipOnKeyPress(digitTwo, chip8);
					break;
				case 0x00A1:
					execSkipOnKeyNotPressed(digitTwo, chip8);
					break;
				default:
					printf("Instruction 0x%04x not recognized!\n", opcode);
					break;
			}
			break;
		case 0xF000:
			switch(byteMaskTwo)
			{
				case 0x0007:
					execSetRegToTimerVal(digitTwo, chip8);
					break;
				case 0x000A:
					execGetKey(digitTwo, chip8);
					break;
				case 0x0015:
					execSetDelayTimerToReg(digitTwo, chip8);
					break;
				case 0x0018:
					execSetSoundTimerToReg(digitTwo, chip8);
					break;
				case 0x001E:
					execAddRegToIndex(digitTwo, chip8);
					break;
				case 0x0029:
					execSetIndexToFont(digitTwo, chip8);
					break;
				case 0x0033:
					execConvertIntToBCD(digitTwo, chip8);
					break;
				case 0x0055:
					execStoreRegInMem(digitTwo, chip8);
					break;
				case 0x0065:
					execLoadRegFromMem(digitTwo, chip8);
					break;
				default:
					printf("Instruction 0x%04x not recognized!\n", opcode);
					break;
			}
			break;
		default:
			printf("Instruction 0x%04x not recognized!\n", opcode);
			break;
	}
}

bool loadROMIntoMem(char *fileName, struct Hardware *chip8)
{
	FILE *ROM;
	bool success;
	int memByte = PC_START;
	uint8_t byteData;

	// add if statement that checks if filename is valid then runs so it will no longer segfault

	ROM = fopen(fileName, "rb");

	if(ROM == NULL)
	{
		success = false;
		printf("Failed to open chip8 ROM file!\n");
	}
	else
	{
		success = true;

		while(fread(&byteData, sizeof(uint8_t), 1, ROM) == 1)
		{
			chip8->RAM[memByte] = byteData;
			memByte++;
		}

		fclose(ROM);
	}

	return success;
}

void updateTimers(struct Hardware *chip8)
{
	static uint8_t cyclesSinceLastTick = 0;

	cyclesSinceLastTick++;

	if(cyclesSinceLastTick == 8)
	{
		cyclesSinceLastTick = 0;
		chip8->Timers.delay -= (chip8->Timers.delay > 0) ? 1 : 0;
		chip8->Timers.sound -= (chip8->Timers.sound > 0) ? 1 : 0;
	}

	if(chip8->Timers.sound > 0)
	{
		playSound();
	}
}

void cycleTiming(struct Hardware *chip8) // times cycles to prespecified delay (see macros in instructions.h)
{
	uint32_t timeBetween = (SDL_GetTicks() - chip8->Timers.lastCycleTime);
	int32_t timeToSleep = CLOCK_TARGET_TIME - timeBetween;

	if(timeToSleep > 0 && timeToSleep <= CLOCK_TARGET_TIME)
	{
		SDL_Delay(timeToSleep);
	}

	updateTimers(chip8); // could be improved with multithreading, for now this works!
	chip8->Timers.lastCycleTime = SDL_GetTicks();
}
