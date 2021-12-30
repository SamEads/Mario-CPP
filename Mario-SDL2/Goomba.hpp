#ifndef GOOMBA_HPP
#define GOOMBA_HPP

#include "Enemy.hpp"

class Player;

class Goomba : public Enemy
{
	private:
	public:
		Goomba(Level* _level);
		void update();
		void hurtPlayer(Player* player);
		void hurtEnemy(Player* player);
		int topClip = 24;
		int stompTimer = 0;
};

#endif