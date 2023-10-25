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
