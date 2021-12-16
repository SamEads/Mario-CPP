#ifndef MARIO_HPP
#define MARIO_HPP

#include "Entity.hpp"
#include "Functions.hpp"
#include <SDL.h>

class Game;

class Mario : public Entity
{
	private:
		// Variables
		Game* game;
	public:
		// Constructor
		Mario(Game* _game);
		// Methods
		void animate();
		void draw();
		void update();
		// Variables
		bool isSkidding = false;
		bool isGrounded = false;
		bool isCrouching = false;
		int pmeterLevel = 112;
		int pmeterMax = 112;
};

#endif