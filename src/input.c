#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "input.h"

bool handleInput(void)
{
	bool quit = false;
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			quit = true;
			break;
		default:
			quit = false;
			break;
	}

	return quit;
}
