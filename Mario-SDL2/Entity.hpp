#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Core.hpp"
#include <iostream>
#include <SDL.h>

class Level;
class Game;

class Entity
{
	protected:
		void updatePosition();
	public:
		std::string identifier = "E";
		virtual void update()
		{
			std::cout << "DEFAULT ENTITY ROUTINE" << std::endl;
		};
		virtual void collide();
		virtual void draw(SDL_Texture* texture, Game* game, float x, float y);
		Rect getHitbox();
		SDL_RendererFlip flipSpr = SDL_FLIP_NONE;
		float imgX = 0;
		float imgY = 0;
		Vector2 position;
		Vector2 spd;
		Vector2 vel;
		Level* level;
		Game* game;
		SDL_Texture* texture;
		bool bumpedHead = false;

		// Animations
		float curFrame = 0;
		Animation curAnim;

		// Hitbox
		int topClip = 18;
		int leftClip = 3;
		int rightClip = 3;
};

#endif // !ENTITY_HPP