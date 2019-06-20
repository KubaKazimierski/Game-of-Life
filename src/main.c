#include "SDL.h"
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argc, char **argv)
{
	bool quit = false;
	int exit_code = EXIT_SUCCESS;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("[SDL] Unable to initialize: %s\n", SDL_GetError());
		exit_code = EXIT_FAILURE;
		goto EXIT;
	}

	window = SDL_CreateWindow("Game of Life",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  WINDOW_WIDTH,
							  WINDOW_HEIGHT,
							  SDL_WINDOW_OPENGL);

	if(window == NULL)
	{
		SDL_Log("[SDL] Couldn't create window: %s", SDL_GetError());
		exit_code = EXIT_FAILURE;
		goto SDLQUIT;
	}

	renderer = SDL_CreateRenderer(window,
								  -1,
								  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL)
	{
		SDL_Log("[SDL] Couldn't create renderer: %s", SDL_GetError());
		exit_code = EXIT_FAILURE;
		goto DESTROY_WINDOW;
	}


	while(!quit)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
				{
					quit = true;
				}
			}

			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);
		}
	}

DESTROY_RENDERER:
	SDL_DestroyRenderer(renderer);
DESTROY_WINDOW:
	SDL_DestroyWindow(window);
SDLQUIT:
	SDL_Quit();
EXIT:
	return exit_code;
}