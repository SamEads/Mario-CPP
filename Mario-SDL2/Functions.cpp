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
	sizeRect.x = x - floor(game->level->camPos.x);
	sizeRect.y = y;
	SDL_RenderCopyEx(game->renderer, game->tilesTexture, &srcRect, &sizeRect, 0, NULL, SDL_FLIP_NONE);
}

void drawText(int x, int y, std::string msg/*, align alignStyle*/)
{
	SDL_Surface* surf;
	SDL_Texture* tex;
	SDL_Rect rect;
	surf = TTF_RenderText_Solid(game->font, msg.c_str(), { 255, 255, 255 });
	tex = SDL_CreateTextureFromSurface(game->renderer, surf);
	int extraWidth = 0;
	/*if (alignStyle == right)
	{
		extraWidth -= surf->w;
	}
	else if (alignStyle == center)
	{
		extraWidth -= surf->w / 2;
	}*/
	rect.x = x + extraWidth;
	rect.y = y;
	rect.w = surf->w;
	rect.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(game->renderer, tex, NULL, &rect);
	SDL_DestroyTexture(tex);
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