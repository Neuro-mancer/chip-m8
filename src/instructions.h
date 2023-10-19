#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "hardware.h"

void execClearDisplay(struct Hardware *chip8);
void execJump(uint16_t addr, struct Hardware *chip8);
void execSetReg(uint8_t value, uint8_t regNum, struct Hardware *chip8);
void execAddValToReg(uint8_t value, uint8_t regNum, struct Hardware *chip8);
void execSetIndexReg(uint16_t addr, struct Hardware *chip8);
void execDraw(uint8_t regNumX, uint8_t regNumY, uint8_t height, struct Hardware *chip8);
void execRet(struct Hardware *chip8);
void execSubroutine(uint16_t addr, struct Hardware *chip8);
void execSkipEqual(uint8_t value, uint8_t regNum, struct Hardware *chip8);
void execSkipNotEqual(uint8_t value, uint8_t regNum, struct Hardware *chip8);
void execSkipRegEqual(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execSetReg(uint8_t value, uint8_t regNum, struct Hardware *chip8);
void execAddValToReg(uint8_t value, uint8_t regNum, struct Hardware *chip8);
void execSetRegXToRegY(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execOR(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execAND(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execXOR(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execAdd(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execSubtYFromX(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execSubtXFromY(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execShiftRight(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execShiftLeft(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execSkipRegNotEqual(uint8_t regNumX, uint8_t regNumY, struct Hardware *chip8);
void execJumpOffset(uint16_t addr, struct Hardware *chip8);
void execGenRandom(uint8_t value, uint8_t regNum, struct Hardware *chip8);

#endif // INSTRUCTIONS_H
