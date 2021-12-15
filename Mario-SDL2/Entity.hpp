#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Functions.h"
#include <iostream>
#include <SDL.h>

class Game;

class Entity
{
	public:
		SDL_RendererFlip flipSpr = SDL_FLIP_NONE;
		float imgX = 0;
		float imgY = 0;
		Vector2 position;
		Vector2 spd;
		virtual void update()
		{
			position.x++;
			std::cout << position.x << std::endl;
		};
		virtual void draw(SDL_Texture* texture, Game* game, float x, float y);
};

#endif // !ENTITY_HPP