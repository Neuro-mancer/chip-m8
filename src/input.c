#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "input.h"

bool handleInput(void)
{
	bool quit = false;
	SDL_Event event;
	SDL_PollEvent(&event);

	while(SDL_PollEvent(&event) != 0)
	{
		if(event.type == SDL_QUIT)
		{
			quit = true;
		}
	}

	return quit;
}

bool checkKeyPress(uint8_t keyCodeChip8)
{
	bool keyPressed = false;
	uint8_t keyCodePressed;
	SDL_Event event;
	SDL_PollEvent(&event);

	while(SDL_PollEvent(&event) != 0)
	{
		if(event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.scancode)
			{
				case SDL_SCANCODE_1:
					keyCodePressed = 0x1;
					break;
				case SDL_SCANCODE_2:
					keyCodePressed = 0x2;
					break;
				case SDL_SCANCODE_3:
					keyCodePressed = 0x3;
					break;
				case SDL_SCANCODE_4:
					keyCodePressed = 0xC;
					break;
				case SDL_SCANCODE_Q:
					keyCodePressed = 0x4;
					break;
				case SDL_SCANCODE_W:
					keyCodePressed = 0x5;
					break;
				case SDL_SCANCODE_E:
					keyCodePressed = 0x6;
					break;
				case SDL_SCANCODE_R:
					keyCodePressed = 0xD;
					break;
				case SDL_SCANCODE_A:
					keyCodePressed = 0x7;
					break;
				case SDL_SCANCODE_S:
					keyCodePressed = 0x8;
					break;
				case SDL_SCANCODE_D:
					keyCodePressed = 0x9;
					break;
				case SDL_SCANCODE_F:
					keyCodePressed = 0xE;
					break;
				case SDL_SCANCODE_Z:
					keyCodePressed = 0xA;
					break;
				case SDL_SCANCODE_X:
					keyCodePressed = 0x0;
					break;
				case SDL_SCANCODE_C:
					keyCodePressed = 0xB;
					break;
				case SDL_SCANCODE_V:
					keyCodePressed = 0xF;
					break;
				default:
					break;
			}
		}
	}

	if(keyCodeChip8 == keyCodePressed)
	{
		keyPressed = true;
	}

	return keyPressed;
}

