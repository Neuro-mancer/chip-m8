#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512
#define SCREEN_SCALE 16

bool graphicsInit(void);
void graphicsCleanup(void);
void clearScreen(void);
void drawPixel(unsigned int x, unsigned int y, bool pixelOn);
bool loadSound(void);
void playSound(void);

#endif // GRAPHICS_H
