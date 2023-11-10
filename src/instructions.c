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
}

void execRet(struct Hardware *chip8)
{
	chip8->PC = stackPop(chip8);
}

void execJump(uint16_t addr, struct Hardware *chip8)
{
	chip8->PC = addr; 
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
	chip8->PC += (chip8->V[regNumX] == chip8->V[regNumY]) ? 2 : 0;
}

void execSetReg(uint8_t value, uint8_t regNum, struct Hardware *chip8)
{
	chip8->V[regNum] = value;
}

void execAddValToReg(uint8_t value, uint8_t regNum, struct Hardware *chip8)
{
	chip8->V[regNum] += value;
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
	unsigned int result = chip8->V[regNumX] + chip8->V[regNumY];
	chip8->V[regNumX] = chip8->V[regNumX] + chip8->V[regNumY];
	chip8->V[0xF] = result > 255;
}

void execSubtYFromX(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	unsigned int regXOld = chip8->V[regNumX];
	chip8->V[regNumX] -= chip8->V[regNumY];
	chip8->V[0xF] = regXOld > chip8->V[regNumY];
}

void execSubtXFromY(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8)
{
	chip8->V[regNumX] = chip8->V[regNumY] - chip8->V[regNumX];
	chip8->V[0xF] = chip8->V[regNumY] > chip8->V[regNumX];
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
	chip8->PC += (chip8->V[regNumX] != chip8->V[regNumY]) ? 2 : 0;
}

void execSetIndexReg(uint16_t addr, struct Hardware *chip8)
{
	chip8->I = addr;
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

void execSkipOnKeyPress(uint8_t regNum, struct Hardware *chip8)
{
	chip8->PC += chip8->keyBuffer[chip8->V[regNum]] ? 2 : 0;
}

void execSkipOnKeyNotPressed(uint8_t regNum, struct Hardware *chip8)
{
	chip8->PC += !(chip8->keyBuffer[chip8->V[regNum]]) ? 2 : 0;
}

void execSetRegToTimerVal(uint8_t regNum, struct Hardware *chip8)
{
	chip8->V[regNum] = chip8->Timers.delay;
}

void execSetDelayTimerToReg(uint8_t regNum, struct Hardware *chip8)
{
	chip8->Timers.delay = chip8->V[regNum];
}

void execSetSoundTimerToReg(uint8_t regNum, struct Hardware *chip8)
{
	chip8->Timers.sound = chip8->V[regNum];
}

void execAddRegToIndex(uint8_t regNum, struct Hardware *chip8)
{
	unsigned int result = chip8->I + chip8->V[regNum];
	chip8->I = chip8->I + chip8->V[regNum];
	chip8->V[0xF] = result > 0xFFF; // add macro for 0xFFF?
	/* 0xFFF is the last addressable memory location for our 4K of RAM
	// the flag seems to be optional behavior not found on original COSMAC interpreter
	// it's nonetheless important */
}

void execGetKey(uint8_t regNum, struct Hardware *chip8)
{
	chip8->PC -= chip8->keyBuffer[chip8->V[regNum]] ? 0 : 2;
}

void execSetIndexToFont(uint8_t regNum, struct Hardware *chip8)
{
	uint8_t fontChar = chip8->V[regNum] & 0x0F;
	uint16_t fontMemLocation;

	fontMemLocation = 0x50 + (fontChar * 5);
	chip8->I = fontMemLocation;
}

void execConvertIntToBCD(uint8_t regNum, struct Hardware *chip8)
{
	uint8_t ones = chip8->V[regNum] % 10;
	uint8_t tens = (chip8->V[regNum] / 10) % 10;
	uint8_t hundreds = (chip8->V[regNum] / 100) % 10;

	chip8->RAM[chip8->I] = hundreds;
	chip8->RAM[chip8->I + 1] = tens;
	chip8->RAM[chip8->I + 2] = ones;
}

// note: these instructions follow the modern interpreter routine storing
// temporary index in for loop, instead of incrementing the I addr register
void execStoreRegInMem(uint8_t regNum, struct Hardware *chip8)
{
	for(int reg = 0; reg <= regNum; reg++)
	{
		chip8->RAM[chip8->I + reg] = chip8->V[reg];
	}
}

void execLoadRegFromMem(uint8_t regNum, struct Hardware *chip8)
{
	for(int reg = 0; reg <= regNum; reg++)
	{
		chip8->V[reg] = chip8->RAM[chip8->I + reg];
	}
}
