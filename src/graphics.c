#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Mix_Chunk *tone = NULL;

bool graphicsInit(void) {
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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
			else
			{
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer was unable to initialize. SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}
	
	return success;
}

bool loadSound(void)
{
	bool success = true;

	tone = Mix_LoadWAV("sound_fx/tone.wav");

	if(tone == NULL)
	{
		success = false;
		printf("Failed to load sound_fx/tone.wav\n");
	}

	Mix_MasterVolume(20);

	return success;
}

void playSound(void)
{
	Mix_PlayChannel(0, tone, 0);
}
void clearScreen(struct Hardware *chip8)
{
	SDL_SetRenderDrawColor(renderer, chip8->DisplayColors.background[0], chip8->DisplayColors.background[1], chip8->DisplayColors.background[2], 255); // background
	SDL_RenderClear(renderer);
}

void drawPixel(unsigned int x, unsigned int y, bool pixelOn, struct Hardware *chip8)
{
	unsigned int transformedX = x * SCREEN_SCALE;
	unsigned int transformedY = y * SCREEN_SCALE;

	SDL_Rect currentPixel = {
		transformedX,
		transformedY,
		SCREEN_SCALE,
		SCREEN_SCALE,	
	};

	if(pixelOn)
	{
		SDL_SetRenderDrawColor(renderer, chip8->DisplayColors.foreground[0], chip8->DisplayColors.foreground[1], chip8->DisplayColors.foreground[2], 255); // foreground
		//
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, chip8->DisplayColors.background[0], chip8->DisplayColors.background[1], chip8->DisplayColors.background[2], 255); // background
	}

	SDL_RenderFillRect(renderer, &currentPixel);
}

void graphicsCleanup(void)
{
	Mix_FreeChunk(tone);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_Quit();
	SDL_Quit();
}
