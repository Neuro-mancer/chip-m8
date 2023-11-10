#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool graphicsInit(void)
{
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initalized. SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(window == NULL)
		{
			printf("Unable to create window. SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL)
			{
				printf("Unable to create renderer. SDL Error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}
	
	return success;
}

void clearScreen(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void drawPixel(unsigned int x, unsigned int y, bool pixelOn)
{
	unsigned int transformedX = x * SCREEN_SCALE;
	unsigned int transformedY = y * SCREEN_SCALE;

	SDL_Rect currentPixel = {
		transformedX,
		transformedY,
		SCREEN_SCALE,
		SCREEN_SCALE,	
	};

	// consider optimizing this function by making sure it doesn't redraw if a pixel is *ALREADY* that color??
	
	if(pixelOn)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
	}

	SDL_RenderFillRect(renderer, &currentPixel);
	SDL_RenderPresent(renderer);
}

void graphicsCleanup(void)
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
