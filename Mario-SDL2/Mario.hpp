#ifndef MARIO_HPP
#define MARIO_HPP

#include "Functions.h"
#include <SDL.h>

class Game;

class Mario
{
	private:
		Game* game;
	public:
		// Constructor
		Mario(Game* _game);
		// Methods
		void update();
		void draw();
		// Variables
		bool isGrounded = false;
		Vector2 position;
		Vector2 spd;
};

#endif