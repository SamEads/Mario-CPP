#include "Enemy.hpp"
#include "Game.hpp"

Enemy::Enemy(Level* _level)
{
	// Entity::level = _level;
	// Entity::game = Entity::level->game;
	texture = game->foesTexture;
}

void Enemy::hurtEnemy(Player* player)
{
	player->spd.y = -3;
	player->isJumping = false;
	die();
}

void Enemy::hurtPlayer(Player* player)
{
	// TODO: implement
}

void Enemy::die()
{
	std::vector<Entity*>::iterator iterator = find(game->level->entities.begin(), game->level->entities.end(), this);
	game->level->entities.erase(iterator);
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
	for (Entity* entity : game->level->entities)
	{
		Enemy* enemy = dynamic_cast<Enemy*> (entity);
		if (entity == enemy)
		{
			if ((spd.x > 0 && enemy->spd.x < 0)
			|| (spd.x < 0 && enemy->spd.x > 0))
			{
				auto myHitbox = getHitbox();
				auto partnerHitbox = enemy->getHitbox();
				if (getHitbox().intersects(enemy->getHitbox()))
				{
					spd.x = -spd.x;
					enemy->spd.x = -enemy->spd.x;
				}
			}
		}
	}
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