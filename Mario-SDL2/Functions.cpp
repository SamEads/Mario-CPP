#include "Functions.hpp"
#include "Game.hpp"

void initFunctions(Game *_game)
{
	game = _game;
}

void drawTile(int x, int y, int tileX, int tileY)
{
	SDL_Rect srcRect;
	SDL_Rect sizeRect;
	srcRect.x = tileX * 16;
	srcRect.y = tileY * 16;
	sizeRect.w = srcRect.w = 16;
	sizeRect.h = srcRect.h = 16;
	sizeRect.x = x - game->level->camPos.x;
	sizeRect.y = y;
	SDL_RenderCopy(game->renderer, game->tilesTexture, &srcRect, &sizeRect);
}