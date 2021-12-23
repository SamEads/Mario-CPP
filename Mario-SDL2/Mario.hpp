#ifndef MARIO_HPP
#define MARIO_HPP

#include "Entity.hpp"
#include "Functions.hpp"
#include <SDL.h>

class Mario : public Entity
{
	public:
		// Constructor
		Mario(Level* _level);
		// Methods
		void animate();
		void draw();
		void update();
		// Variables
		bool isSkidding = false;
		bool isCrouching = false;
		int pmeterLevel = 112;
		int pmeterMax = 112;
};

#endif