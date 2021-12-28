#ifndef GOOMBA_HPP
#define GOOMBA_HPP

#include "Entity.hpp"

class Mario;

class Goomba : public Entity
{
	private:
	public:
		void hurtPlayer(Mario* mario);
		void hurtEnemy(Mario* mario);
};

#endif