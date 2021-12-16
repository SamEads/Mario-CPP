#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Functions.hpp"
#include <iostream>
#include <SDL.h>

class Entity
{
	protected:
		void updatePosition();
	public:
		virtual void update()
		{
			std::cout << "DEFAULT ENTITY ROUTINE" << std::endl;
		};
		virtual void collide();
		virtual void draw(SDL_Texture* texture, Game* game, float x, float y);
		SDL_RendererFlip flipSpr = SDL_FLIP_NONE;
		float imgX = 0;
		float imgY = 0;
		Vector2 position;
		Vector2 spd;
		Animation curAnim;
};

#endif // !ENTITY_HPP