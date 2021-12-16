#include "Entity.hpp"
#include <iostream>
#include "Game.hpp"

void Entity::collide()
{

}

void Entity::updatePosition()
{
	position.x += spd.x;
	position.y += spd.y;
}

void Entity::draw(SDL_Texture* texture, Game* game, float x, float y)
{
	SDL_Rect srcRect;
	SDL_Rect sizeRect;
	SDL_Point centerPoint;
	srcRect.x = floor(imgX) * 32;
	srcRect.y = floor(imgY) * 32;
	sizeRect.w = srcRect.w = 32;
	sizeRect.h = srcRect.h = 32;
	sizeRect.x = (position.x) - game->level->camPos.x;
	sizeRect.y = (position.y) + 1;
	centerPoint.x = 16;
	centerPoint.y = 16;
	SDL_RenderCopyEx(game->renderer, texture, &srcRect, &sizeRect, 0, NULL, flipSpr);
}