#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hardware.h"
#include "instructions.h"

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
	if(chip8->Stack.SP == 0)
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

void initReg(struct Hardware *chip8)
{
	chip8->PC = PC_START;
	chip8->Stack.SP = STACK_START;
	chip8->Timers.delay = DELAY_TIMER_START;
	chip8->Timers.sound = SOUND_TIMER_START;
	memset(chip8->displayBuffer, false, sizeof chip8->displayBuffer);
	printf("Hardware pointers and registers initialized with starting values\n");
}

uint16_t fetch(struct Hardware *chip8)
{
	// note: chip-8 opcodes are stored big endian 
	uint16_t opcodeByteOne, opcodeByteTwo;
	uint16_t opcode; // look into bitcodes for instructions, might be useful

	opcodeByteOne = chip8->RAM[chip8->PC]; 
	opcodeByteTwo = chip8->RAM[chip8->PC + 1];

	opcode = (opcodeByteOne << 8) | opcodeByteTwo;
	printf("Opcode to be executed: 0x%04x\t|\tcurrent instruction address: 0x%04x\n", opcode, chip8->PC);
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
				case 0x000E:
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
					break;
				case 0x00A1:
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
					break;
				case 0x000A:
					break;
				case 0x0015:
					break;
				case 0x0018:
					break;
				case 0x001E:
					break;
				case 0x0029:
					break;
				case 0x0033:
					break;
				case 0x0055:
					break;
				case 0x0065:
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
	}

	fclose(ROM);
	return success;
}
