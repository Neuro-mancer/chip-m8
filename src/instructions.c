#include "instructions.h"
#include "hardware.h"
#include "graphics.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void execClearDisplay(struct Hardware *chip8)
{
	memset(chip8->displayBuffer, false, sizeof chip8->displayBuffer);
	clearScreen();
	printf("Clear Screen\n");
}

void execRet(struct Hardware *chip8)
{
	chip8->PC = stackPop(chip8);
}

void execJump(uint16_t addr, struct Hardware *chip8)
{
	chip8->PC = addr; 
	printf("Jumping to address: 0x%04x\n", chip8->PC);
}

void execSubroutine(uint16_t addr, struct Hardware *chip8)
{
	stackPush(chip8->PC, chip8);
	chip8->PC = addr;
}

void execSkipEqual(uint8_t value, uint8_t regNum, struct Hardware *chip8) 
{
	chip8->PC += (value == chip8->V[regNum]) ? 2 : 0;
}

void execSkipNotEqual(uint8_t value, uint8_t regNum, struct Hardware *chip8)
{
	chip8->PC += (value != chip8->V[regNum]) ? 2 : 0;
}

void execSkipRegEqual(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->PC += (regNumX == regNumY) ? 2 : 0;
}

void execSetReg(uint8_t value, uint8_t regNum, struct Hardware *chip8)
{
	chip8->V[regNum] = value;
	printf("Set Value: 0x%04x, Register: V0x%04x\n", chip8->V[regNum], regNum);
}

void execAddValToReg(uint8_t value, uint8_t regNum, struct Hardware *chip8)
{
	chip8->V[regNum] += value;
	printf("Add Value: 0x%04x, Final Value: 0x%04x, Register: V0x%04x\n", value, chip8->V[regNum], regNum);
}

void execSetRegXToRegY(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] = chip8->V[regNumY];
}

void execOR(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] |= chip8->V[regNumY];
}

void execAND(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] &= chip8->V[regNumY];
}

void execXOR(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] ^= chip8->V[regNumY];
}

void execAdd(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] += chip8->V[regNumY];
}

void execSubtYFromX(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] -= chip8->V[regNumY];
}

void execSubtXFromY(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] = chip8->V[regNumY] - chip8->V[regNumX];
}

void execShiftRight(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	const uint8_t bitMask = 0b00000001;
	bool LSBFlag;
	chip8->V[regNumX] = chip8->V[regNumY];
	LSBFlag = bitMask & chip8->V[regNumX];
	chip8->V[regNumX] >>= 1;
	chip8->V[0xF] = LSBFlag;
}

void execShiftLeft(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	const uint8_t bitMask = 0b10000000;
	bool MSBFlag;

	chip8->V[regNumX] = chip8->V[regNumY];
	MSBFlag = (bitMask & chip8->V[regNumX]) >> 7;
	chip8->V[regNumX] <<= 1;
	chip8->V[0xF] = MSBFlag;
}

void execSkipRegNotEqual(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->PC += (regNumX != regNumY) ? 2 : 0;
}

void execSetIndexReg(uint16_t addr, struct Hardware *chip8)
{
	chip8->I = addr;
	printf("Index Address: 0x%04x\n", chip8->I);
}

void execJumpOffset(uint16_t addr, struct Hardware *chip8)
{
	chip8->PC = addr + chip8->V[0];
}

void execGenRandom(uint8_t value, uint8_t regNum, struct Hardware *chip8)
{
	uint8_t randomNum;

	srand(time(NULL));
	randomNum = rand() % 0xFF;
	chip8->V[regNum] = randomNum & value;
}

void execDraw(uint8_t regNumX, uint8_t regNumY, uint8_t height, struct Hardware *chip8)
{
	uint8_t x, y;
	uint8_t spriteData;

	bool pixelNextState;
	bool pixelToBeDrawn;
	bool pixelCurrent;

	x = chip8->V[regNumX] % DISPLAY_WIDTH;
	y = chip8->V[regNumY] % DISPLAY_HEIGHT;

	chip8->V[0xF] = 0;

	for(int spriteByte = 0; spriteByte < height; spriteByte++)
	{
		spriteData = chip8->RAM[spriteByte + chip8->I];

		for(int spriteBit = 0; spriteBit < 8; spriteBit++)
		{
			if(x + spriteBit < DISPLAY_WIDTH && y + spriteByte < DISPLAY_HEIGHT)
			{
				pixelCurrent = chip8->displayBuffer[y + spriteByte][x + spriteBit];
				pixelToBeDrawn = spriteData & (1 << (7 - spriteBit));
				pixelNextState = pixelToBeDrawn ^ pixelCurrent;

				if(pixelToBeDrawn && pixelCurrent)
				{
					chip8->V[0xF] = 1;
				}

				drawPixel(x + spriteBit, y + spriteByte, pixelNextState);
			}
		}
	}
}
