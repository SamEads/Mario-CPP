#ifndef MARIO_HPP
#define MARIO_HPP

#include "Entity.hpp"
#include "Functions.hpp"
#include <SDL.h>

enum Powerup
{
	SMALL = 0,
	BIG = 1
};

class Mario : public Entity
{
	public:
		// Constructor
		Mario(Level* _level);
		// Methods
		void animate();
		void update();
		// Variables
		bool isSkidding = false;
		bool isCrouching = false;
		int pmeterLevel = 112;
		int pmeterMax = 112;
		Powerup powerup = SMALL;
};

#endif