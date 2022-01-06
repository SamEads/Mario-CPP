#include "Goomba.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "Game.hpp"
#include "AssetManager.hpp"

Goomba::Goomba(Level* _level) : Enemy(_level)
{
}

void Goomba::update()
{
	Enemy::update();
	if (!alive)
	{
		curAnim.frames = { 2 };
		spd.x = 0;
		stompTimer++;
		if (stompTimer >= 20)
		{
			die();
		}
	}
}

void Goomba::hurtEnemy(Player* player)
{
	playSound("squish", false, true);
	player->spd.y = -3;
	player->isJumping = false;
	game->score += 100;
	alive = false;
}

void Goomba::hurtPlayer(Player* player)
{
	Enemy::hurtPlayer(player);
}