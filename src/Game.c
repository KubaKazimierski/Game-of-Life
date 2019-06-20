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

#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

static void randomize_board(enum Cell_State *board)
{
	srand((unsigned int) time(NULL));

	enum Cell_State *ccell = board; //current cell
	for(size_t i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; ++i, ++ccell)
	{
		*ccell = rand() % 2;
	}
}

static unsigned int neighbour_number(const enum Cell_State *board, const int ox, const int oy)
{
	unsigned int result = 0;

	for(int i = 0, y = oy - 1; i < 3; ++i, ++y)
	{
		int cx, cy;

		//Looping of y axis
		if(y < 0)
		{
			cy = BOARD_HEIGHT + y;
		}
		else if(y > BOARD_HEIGHT - 1)
		{
			cy = BOARD_HEIGHT - y;
		}
		else
		{
			cy = y;
		}

		for(int j = 0, x = ox - 1; j < 3; ++j, ++x)
		{
			//Looping of x axis
			if(x < 0)
			{
				cx = BOARD_WIDTH + x;
			}
			else if(x > BOARD_WIDTH - 1)
			{
				cx = BOARD_WIDTH - x;
			}
			else
			{
				cx = x;
			}

			if(cx == ox && cy == oy)
				continue;

			result += *(board + (cx + BOARD_WIDTH * cy));
		}
	}

	return result;
}

static void update_board(enum Cell_State *board)
{
	size_t board_size = BOARD_WIDTH * BOARD_HEIGHT * sizeof(enum Cell_State);

	enum Cell_State *new_board = malloc(board_size);
	if(new_board == NULL)
	{
		SDL_Log("[Game] Failed to create new board");
		return;
	}

	memset(new_board, 0, board_size);

	for(size_t y = 0; y < BOARD_HEIGHT; ++y)
	{
		for(size_t x = 0; x < BOARD_WIDTH; ++x)
		{
			unsigned int neighbours = neighbour_number(board, x, y);

			if((neighbours == 3 && *(board + (x + BOARD_WIDTH * y)) == DEAD)
			   || ((neighbours == 2 || neighbours == 3) && *(board + (x + BOARD_WIDTH * y)) == ALIVE))
			{
				*(new_board + (x + BOARD_WIDTH * y)) = ALIVE;
			}
			else
			{
				*(new_board + (x + BOARD_WIDTH * y)) = DEAD;
			}
		}
	}

	memcpy(board, new_board, board_size);
	free(new_board);
}

static void handle_events(struct Game *game)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
			{
				game->quit = true;
			} break;
		}
	}
}

static void draw_board(struct Game *game)
{
	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

	SDL_Rect cell = { 0, 0, CELL_SIZE, CELL_SIZE};
	enum Cell_State *ccell_state = game->board; //current cell state

	for(size_t y = 0; y < BOARD_HEIGHT; ++y)
	{
		cell.x = 0;
		for(size_t x = 0; x < BOARD_WIDTH; ++x)
		{
			switch(*ccell_state)
			{
				case DEAD:
				{
					SDL_RenderDrawRect(game->renderer, &cell);
				} break;

				case ALIVE:
				{
					SDL_RenderFillRect(game->renderer, &cell);
				} break;
			}

			++ccell_state;
			cell.x += CELL_SIZE;
		}

		cell.y += CELL_SIZE;
	}

	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
}

static void draw(struct Game *game)
{
	SDL_RenderClear(game->renderer);

	/* Draw here */
	draw_board(game);

	SDL_RenderPresent(game->renderer);
}

/* ---- Game interface ---- */
struct Game* Game_init(SDL_Renderer *renderer)
{
	struct Game *result = malloc(sizeof(struct Game));

	if(result != NULL)
	{
		result->quit = false;
		result->renderer = renderer;
		
		memset(result->board, 0, sizeof result->board);

#ifdef RANDOM
		randomize_board(result->board);
#endif

#ifdef GLIDER
		*(result->board + (6 + BOARD_WIDTH * 5)) = ALIVE;
		*(result->board + (7 + BOARD_WIDTH * 6)) = ALIVE;
		*(result->board + (5 + BOARD_WIDTH * 7)) = ALIVE;
		*(result->board + (6 + BOARD_WIDTH * 7)) = ALIVE;
		*(result->board + (7 + BOARD_WIDTH * 7)) = ALIVE;
#endif
	}

	return result;
}

struct Game* Game_run(struct Game *game)
{
	Uint32 ltime = SDL_GetTicks();

	while(!game->quit)
	{
		if(SDL_GetTicks() - ltime > TICK_TIME)
		{
			update_board(game->board);
			ltime = SDL_GetTicks();
		}

		handle_events(game);
		draw(game);
	}
}

struct Game* Game_free(struct Game *game)
{
	free(game);
}
/* ------------------------ */
