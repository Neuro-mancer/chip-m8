#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "input.h"
#include "hardware.h"

void handleInput(struct Hardware *chip8)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type)
	{
		case SDL_QUIT:
			chip8->STATE = QUIT;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.scancode)
			{
				case SDL_SCANCODE_1:
					chip8->keyBuffer[0x1] = true;
					break;
				case SDL_SCANCODE_2:
					chip8->keyBuffer[0x2] = true;
					break;
				case SDL_SCANCODE_3:
					chip8->keyBuffer[0x3] = true;
					break;
				case SDL_SCANCODE_4:
					chip8->keyBuffer[0xC] = true;
					break;
				case SDL_SCANCODE_Q:
					chip8->keyBuffer[0x4] = true;
					break;
				case SDL_SCANCODE_W:
					chip8->keyBuffer[0x5] = true;
					break;
				case SDL_SCANCODE_E:
					chip8->keyBuffer[0x6] = true;
					break;
				case SDL_SCANCODE_R:
					chip8->keyBuffer[0xD] = true;
					break;
				case SDL_SCANCODE_A:
					chip8->keyBuffer[0x7] = true;
					break;
				case SDL_SCANCODE_S:
					chip8->keyBuffer[0x8] = true;
					break;
				case SDL_SCANCODE_D:
					chip8->keyBuffer[0x9] = true;
					break;
				case SDL_SCANCODE_F:
					chip8->keyBuffer[0xE] = true;
					break;
				case SDL_SCANCODE_Z:
					chip8->keyBuffer[0xA] = true;
					break;
				case SDL_SCANCODE_X:
					chip8->keyBuffer[0x0] = true;
					break;
				case SDL_SCANCODE_C:
					chip8->keyBuffer[0xB] = true;
					break;
				case SDL_SCANCODE_V:
					chip8->keyBuffer[0xF] = true;
					break;
				case SDL_SCANCODE_ESCAPE:
					chip8->STATE = (chip8->STATE == EXECUTE) ? PAUSE : EXECUTE;
					break;
				case SDL_SCANCODE_SPACE:
					chip8->STATE = QUIT;
					break;
				case SDL_SCANCODE_UP:
					if(chip8->emulationSpeed != EMULATION_SPEED_MAX)
					{
						chip8->emulationSpeed += 50;
					}
					break;
				case SDL_SCANCODE_DOWN:
					if(chip8->emulationSpeed != EMULATION_SPEED_MIN)
					{
						chip8->emulationSpeed -= 50;
					}
					break;
				default:
					break;
			}
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.scancode)
			{
				case SDL_SCANCODE_1:
					chip8->keyBuffer[0x1] = false;
					break;
				case SDL_SCANCODE_2:
					chip8->keyBuffer[0x2] = false;
					break;
				case SDL_SCANCODE_3:
					chip8->keyBuffer[0x3] = false;
					break;
				case SDL_SCANCODE_4:
					chip8->keyBuffer[0xC] = false;
					break;
				case SDL_SCANCODE_Q:
					chip8->keyBuffer[0x4] = false;
					break;
				case SDL_SCANCODE_W:
					chip8->keyBuffer[0x5] = false;
					break;
				case SDL_SCANCODE_E:
					chip8->keyBuffer[0x6] = false;
					break;
				case SDL_SCANCODE_R:
					chip8->keyBuffer[0xD] = false;
					break;
				case SDL_SCANCODE_A:
					chip8->keyBuffer[0x7] = false;
					break;
				case SDL_SCANCODE_S:
					chip8->keyBuffer[0x8] = false;
					break;
				case SDL_SCANCODE_D:
					chip8->keyBuffer[0x9] = false;
					break;
				case SDL_SCANCODE_F:
					chip8->keyBuffer[0xE] = false;
					break;
				case SDL_SCANCODE_Z:
					chip8->keyBuffer[0xA] = false;
					break;
				case SDL_SCANCODE_X:
					chip8->keyBuffer[0x0] = false;
					break;
				case SDL_SCANCODE_C:
					chip8->keyBuffer[0xB] = false;
					break;
				case SDL_SCANCODE_V:
					chip8->keyBuffer[0xF] = false;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
