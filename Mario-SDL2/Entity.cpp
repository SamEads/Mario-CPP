#include "Entity.hpp"
#include <iostream>
#include "Game.hpp"
#include "Camera.hpp"

void Entity::draw(SDL_Texture* texture, Game* game, float x, float y)
{
	// Things to draw
	SDL_Rect* srcRect = new SDL_Rect();
	SDL_Rect* sizeRect = new SDL_Rect();
	SDL_Point* centerPoint = new SDL_Point();
	srcRect->x = floor(imgX) * 32;
	srcRect->y = floor(imgY) * 32;
	sizeRect->w = srcRect->w = 32;
	sizeRect->h = srcRect->h = 32;
	sizeRect->x = (position.x) - game->camPos.x;
	sizeRect->y = (position.y);
	centerPoint->x = 16;
	centerPoint->y = 16;
	SDL_RenderCopyEx(game->renderer, game->playerBigTexture, srcRect, sizeRect, 0, NULL, flipSpr);
	delete srcRect;
	delete sizeRect;
	delete centerPoint;
}