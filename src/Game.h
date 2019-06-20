#pragma once
#include "constants.h"
#include <stdbool.h>
#include <SDL.h>

enum Cell_State
{
	DEAD,
	ALIVE,
	DYING
};

struct Game
{
	bool quit;
	SDL_Renderer *renderer;
	enum Cell_State board[BOARD_WIDTH * BOARD_WIDTH];
};

struct Game* Game_init(SDL_Renderer *renderer);
struct Game* Game_run(struct Game *game);
struct Game* Game_free(struct Game *game);