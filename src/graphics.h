#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include "hardware.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512
#define SCREEN_SCALE 16
#define BACK_RED_VAL 0xB8
#define BACK_GREEN_VAL 0xC2
#define BACK_BLUE_VAL 0xB9
#define FORE_RED_VAL 0x38
#define FORE_GREEN_VAL 0x2B
#define FORE_BLUE_VAL 0x26

extern SDL_Renderer *renderer;

bool graphicsInit(void);
void graphicsCleanup(void);
void clearScreen(struct Hardware *chip8);
void drawPixel(unsigned int x, unsigned int y, bool pixelOn, struct Hardware *chip8);
bool loadSound(void);
void playSound(void);

#endif // GRAPHICS_H
