#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "hardware.h"

bool handleInput(struct Hardware *chip8);

#endif // INPUT_H
