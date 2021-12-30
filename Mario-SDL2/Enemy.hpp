#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Player;

class Enemy : public Entity
{
	public:
		bool alive = true;
		Enemy(Level* _level);
		virtual void hurtPlayer(Player* player);
		virtual void hurtEnemy(Player* player);
		virtual void die();
		virtual void update();
};

#endif