#include "Enemy.hpp"
#include "Game.hpp"
#include <iostream>
#include "Player.hpp"
#include "AssetManager.hpp"

Player::Player(Level* _level)
{
	depth = -1;
	texture = getTexture("player/mario");
	identifier = "M";
}

void Player::update()
{
	Entity::update();
	#if DEBUG
	if (game->input->wasJustPressed(SDL_SCANCODE_L))
	{
		if (character == MARIO)
			character = LUIGI;
		else
			character = MARIO;
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_1))
	{
		if (powerup == BIG)
			powerup = SMALL;
		else
			powerup = BIG;
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_N))
	{
		noClip = !noClip;
	}
	if (noClip)
	{
		spd.x = 0;
		spd.y = 0;
		int xMoveDir = game->input->isPressed(SDL_SCANCODE_RIGHT) - game->input->isPressed(SDL_SCANCODE_LEFT);
		int yMoveDir = game->input->isPressed(SDL_SCANCODE_DOWN) - game->input->isPressed(SDL_SCANCODE_UP);
		spd.x = xMoveDir * 8;
		spd.y = yMoveDir * 8;
		position += spd;
		return;
	}
	#endif

	// 8 frame run grace period (fireballing is much more natural)
	if (game->input->isPressed(SDL_SCANCODE_Z))
		runTime = 8;
	else if (runTime > 0)
		--runTime;

	float accel = 0.0625;
	if (character == LUIGI)
		accel /= 1.25;
	float decel = (vel.y == 0) ? 0.0375 : 0.01875;
	float maxSpd = (runTime > 0 && !isCrouching) ? (fullRun) ? 3 : 2.5 : 1.25;
	if (character == LUIGI)
		maxSpd *= 1.1;
	int moveDir = (game->input->isPressed(SDL_SCANCODE_RIGHT) && !game->input->isPressed(SDL_SCANCODE_LEFT)) - game->input->isPressed(SDL_SCANCODE_LEFT);

	if (maxSpd >= 2.5 && fabsf(spd.x) > 1.25 && moveDir != 0 && !isSkidding)
	{
		if (vel.y == 0)
		{
			if (pmeterLevel > 0)
			{
				pmeterLevel--;
				fullRun = false;
			}
			else
			{
				pmeterLevel = 0;
				fullRun = true;
			}
		}
	}
	else
	{
		if (vel.y == 0)
			fullRun = false;
		if (pmeterLevel < pmeterMax)
			pmeterLevel++;
		else
			pmeterLevel = pmeterMax;
	}

	if (pmeterLevel == 0)
	{
		playSound("pmeter", true, false, 0.25f);
	}

	if (vel.y == 0)
	{
		isCrouching = game->input->isPressed(SDL_SCANCODE_DOWN);
	}
	// Set speeds between -1, 0, and 1
	int signSpd = (spd.x > 0) ? 1 : (spd.x < 0) ? -1 : 0;
	isSkidding = false;
	if (moveDir != 0 && (!isCrouching || vel.y != 0))
	{
		if (!isCrouching || vel.y != 0)
		{
			if (spd.x * moveDir < 0)
			{
				isSkidding = true;
			}
			if (vel.y == 0)
				flipSpr = (moveDir < 0) ? FLIP_HORIZONTAL : FLIP_NONE;
			if (spd.x * moveDir < maxSpd)
				spd.x += accel * moveDir;
		}
	}
	// Slow down if no input
	else if (spd.x != 0)
	{
		spd.x -= decel * signSpd;
		if (spd.x * signSpd <= 0)
			spd.x = 0;
	}
	// If above maximum speed, slow down
	if (fabsf(spd.x) >= maxSpd)
	{
		spd.x -= decel * signSpd;
		if (spd.x * signSpd < maxSpd)
			spd.x = maxSpd * signSpd;
	}

	// Apply gravity to Player
	float fallingGravity = 0.375;
	// When full running, give closer to a moon-jump so that more X-distance is covered in the air (Super Player World p-run-esque)
	float jumpingGravity = (!fullRun) ? 0.1875 : 0.125;
	vel.y = (!game->input->isPressed(SDL_SCANCODE_X)) ? fallingGravity : jumpingGravity;
	if (character == LUIGI)
		vel.y *= 0.9;

	// Prevent from gaining a ridiculous amount of downwards momentum
	if (spd.y > 4)
		spd.y = 4;
	spd += vel;

	if ((position.x+8 < 0 && spd.x < 0) || (position.x+8 > (game->level->levelWidth*16)-16 && spd.x > 0))
		spd.x = 0;

	if (spd.y > 0 && position.y >= game->level->levelHeight * 16 + 16)
	{
		spd.y = 0;
		spd.x = 0;
	}

	updatePosition();
	collide();

	if (bumpedHead)
	{
		playSound("bump", false, true, 0.25f);
	}

	if (vel.y == 0 && isJumping)
		isJumping = false;

	// Jump
	if (game->input->wasJustPressed(SDL_SCANCODE_X))
	{
		if (vel.y == 0)
		{
			spd.y = (fullRun) ? -5 : -5 - (fabsf(spd.x) / 4);
			isJumping = true;
			playSound("jump", false, true);
		}
	}

	for (Entity* entity : game->level->entities)
	{
		Enemy* enemy = dynamic_cast<Enemy*> (entity);
		if (entity == enemy)
		{
			if (!enemy->alive)
				continue;

			if (entity->getHitbox().intersects(getHitbox()))
			{
				if (spd.y > 0)
				{
					enemy->hurtEnemy(this);
				}
				else
				{
					enemy->hurtPlayer(this);
				}
			}
		}
	}

	animate();
}

void Player::animate()
{
	texture = (character == LUIGI) ? getTexture("player/luigi") : getTexture("player/mario");
	imgY = (int)powerup;
	switch (powerup)
	{
		default:
			topClip = (isCrouching) ? 20 : 18;
			idleAnim.frames = { 0, 9 };
			walkAnim.frames = { 0, 1 };
			jumpAnim.frames = { 3 };
			skidAnim.frames = { 2 };
			runAnim.frames = { 4, 5 };
			runJumpAnim.frames = { 6 };
			crouchAnim.frames = { 7 };
			break;
		case (BIG):
			topClip = (isCrouching) ? 18 : 8;
			idleAnim.frames = { 0, 11 };
			walkAnim.frames = { 0, 1, 2, 1 };
			jumpAnim.frames = { 4, 2 };
			skidAnim.frames = { 3 };
			runAnim.frames = { 5, 6, 7, 6 };
			runJumpAnim.frames = { 8 };
			crouchAnim.frames = { 9 };
			break;
	}

	if (isCrouching)
		curAnim = crouchAnim;
	else if (vel.y == 0)
	{
		if (isSkidding)
		{
			curAnim = skidAnim;
			playSound("skid", true, false, 0.5f);
		}
		else
		{
			if (spd.x != 0)
			{
				if (!fullRun)
					curAnim = walkAnim;
				else
					curAnim = runAnim;
				if (spd.x != 0)
					curFrame += fabsf(spd.x / 6);
				else
					curFrame = 0;
			}
			else
			{
				curAnim = idleAnim;
				if (game->input->isPressed(SDL_SCANCODE_UP))
					curFrame = 1;
				else
					curFrame = 0;
			}
		}
	}
	else
	{
		if (!fullRun)
			curAnim = jumpAnim;
		else
			curAnim = runJumpAnim;
		curFrame = (isJumping && spd.y < 0) ? 0 : 1;
	}
}