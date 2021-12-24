#include "Enemy.hpp"
#include "Game.hpp"

Enemy::Enemy(Level* _level)
{
	Entity::level = _level;
	Entity::game = Entity::level->game;
	texture = game->foesTexture;
}

void Enemy::hurtEnemy(Mario* mario)
{
	mario->spd.y = -3;
	die();
}

void Enemy::hurtPlayer(Mario* mario)
{
	// TODO: implement
}

void Enemy::die()
{
	std::vector<Entity*>::iterator iterator = find(level->entities.begin(), level->entities.end(), this);
	level->entities.erase(iterator);
}

void Enemy::update()
{
	curAnim.frames = { 0, 1 };
	float lastSpd = spd.x;
	vel.y = 0.2;
	spd.x += vel.x;
	spd.y += vel.y;
	if (spd.y > 4)
		spd.y = 4;
	updatePosition();
	collide();
	if (spd.x == 0)
	{
		if (lastSpd == 0)
			spd.x = -0.5f;
		else
			spd.x = -lastSpd;
	}
	if (position.x < 0)
	{
		if (spd.x < 0)
		{
			spd.x = fabsf(spd.x);
		}
	}
	curFrame += fabsf(spd.x) / 6;
}