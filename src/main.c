/*
MIT License

Copyright (c) 2019 Jakub Kazimierski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include "constants.h"
#include "Game.h"

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

	struct Game *game = Game_init(renderer);
	if(game == NULL)
	{
		SDL_Log("[Game] Failed to initialize");
		exit_code = EXIT_FAILURE;
		goto DESTROY_RENDERER;
	}

	Game_run(game);
	Game_free(game);

DESTROY_RENDERER:
	SDL_DestroyRenderer(renderer);
DESTROY_WINDOW:
	SDL_DestroyWindow(window);
SDLQUIT:
	SDL_Quit();
EXIT:
	return exit_code;
}