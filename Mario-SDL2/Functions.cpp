#include "Core.hpp"
#include "Game.hpp"

Game *game = NULL;
void initFunctions(Game *_game)
{
	game = _game;
}

void drawTile(int x, int y, int tileX, int tileY, bool followCam)
{
	SDL_Rect srcRect;
	SDL_Rect sizeRect;
	srcRect.x = tileX * 16;
	srcRect.y = tileY * 16;
	sizeRect.w = srcRect.w = 16;
	sizeRect.h = srcRect.h = 16;
	sizeRect.x = x;
	if (followCam)
		sizeRect.x -= floor(game->level->camPos.x);
	sizeRect.y = y;
	if (6 == tileX && 0 == tileY)
	{
		int floatTick = floor(game->shineTick);
		srcRect.x += floatTick * 16;
	}
	SDL_RenderCopyEx(game->renderer, game->tilesTexture, &srcRect, &sizeRect, 0, NULL, SDL_FLIP_NONE);
}

void trueMouseCoordinates(SDL_Renderer* renderer, SDL_Window* window, int* logicalMouseX, int* logicalMouseY)
{
	int realMouseX, realMouseY;
	int wWidth, wHeight;
	int rLogicalWidth, rLogicalHeight;
	int rRealWidth, rRealHeight;
	float rScaleX, rScaleY;
	int rMidpointY, wMidpointY;
	int rMidpointX, wMidpointX;
	int rY, rX;

	SDL_GetMouseState(&realMouseX, &realMouseY);
	SDL_GetWindowSize(window, &wWidth, &wHeight);
	wMidpointY = wHeight / 2;
	wMidpointX = wWidth / 2;

	SDL_RenderGetLogicalSize(renderer, &rLogicalWidth, &rLogicalHeight);
	SDL_RenderGetScale(renderer, &rScaleX, &rScaleY);
	rRealWidth = (float)rLogicalWidth * (float)rScaleX;
	rRealHeight = (float)rLogicalHeight * (float)rScaleY;
	rMidpointY = rRealHeight / 2;
	rMidpointX = rRealWidth / 2;
	rY = wMidpointY - rMidpointY;
	rX = wMidpointX - rMidpointX;

	int adjustedMouseY = realMouseY - rY; // takes into account any border when keeping aspect ratio
	int adjustedMouseX = realMouseX - rX;
	*logicalMouseX = (float)adjustedMouseX / (float)rRealWidth * (float)rLogicalWidth;
	*logicalMouseY = (float)adjustedMouseY / (float)rRealHeight * (float)rLogicalHeight;

}