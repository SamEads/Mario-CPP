#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Mario;

class Enemy : public Entity
{
	public:
		Enemy(Level* _level);
		virtual void hurtPlayer(Mario* mario);
		virtual void hurtEnemy(Mario* mario);
		virtual void die();
		void update();
};

#endif