#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

static void randomize_board(enum Cell_State *board)
{
	srand(time(NULL));

	enum Cell_State *ccell = board; //current cell
	for(size_t i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; ++i, ++ccell)
	{
		*ccell = rand() % 2;
	}
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
			}
		}
	}
}

static void draw_board(struct Game *game)
{
	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

	SDL_Rect cell = { 0, 0, CELL_SIZE, CELL_SIZE};
	enum Cell_State *ccell_state = game->board; //current cell state

	for(int y = 0; y < BOARD_HEIGHT; ++y)
	{
		cell.x = 0;
		for(int x = 0; x < BOARD_WIDTH; ++x)
		{
			switch(*ccell_state)
			{
				case DEAD:
				{
					SDL_RenderDrawRect(game->renderer, &cell);
				} break;

				case ALIVE:
				case DYING:
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
		
		memset(result->board, DEAD, sizeof result->board);
		randomize_board(result->board);
	}

	return result;
}

struct Game* Game_run(struct Game *game)
{
	while(!game->quit)
	{
		handle_events(game);
		draw(game);
	}
}

struct Game* Game_free(struct Game *game)
{
	free(game);
}
/* ------------------------ */