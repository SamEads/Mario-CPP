#include "Mario.hpp"
#include "Game.hpp"
#include <iostream>
#include <SDL_mixer.h>

Animation idleAnim;
Animation walkAnim;
Animation runAnim;
Animation runJumpAnim;
Animation skidAnim;
Animation jumpAnim;
Animation crouchAnim;

Mario::Mario(Level* _level)
{
	if (Entity::level != nullptr)
	{
		std::cout << "WHAT THE FUCK" << std::endl;
	}
	Entity::level = _level;
	game = Entity::level->game;
	idleAnim.frames = { 0, 11 };
	walkAnim.frames = { 0, 1, 2, 1 };
	jumpAnim.frames = { 4, 2 };
	skidAnim.frames = { 3 };
	runAnim.frames = { 5, 6, 7, 6 };
	runJumpAnim.frames = { 8 };
	crouchAnim.frames = { 9 };
}

bool fullRun = false;
bool noClip = false;
void Mario::update()
{
	if (game->input->wasJustPressed(SDL_SCANCODE_N))
	{
		noClip = !noClip;
	}
	if (noClip)
	{
		int xMoveDir = game->input->isPressed(SDL_SCANCODE_RIGHT) - game->input->isPressed(SDL_SCANCODE_LEFT);
		int yMoveDir = game->input->isPressed(SDL_SCANCODE_DOWN) - game->input->isPressed(SDL_SCANCODE_UP);
		position.x += xMoveDir * 4;
		position.y += yMoveDir * 4;
		return;
	}
	float accel = 0.0625;
	float decel = (vel.y == 0) ? 0.0375 : 0.01875;
	float maxSpd = (game->input->isPressed(SDL_SCANCODE_Z) && !isCrouching) ? (fullRun) ? 3 : 2.5 : 1.25;
	int moveDir = game->input->isPressed(SDL_SCANCODE_RIGHT) - game->input->isPressed(SDL_SCANCODE_LEFT);

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
				flipSpr = (moveDir < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
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

	// Apply gravity to Mario
	float fallingGravity = 0.375;
	// When full running, give closer to a moon-jump so that more X-distance is covered in the air (Super Mario World p-run-esque)
	float jumpingGravity = (!fullRun) ? 0.1875 : 0.125;
	vel.y = (!game->input->isPressed(SDL_SCANCODE_X)) ? fallingGravity : jumpingGravity;
	spd.x += vel.x;
	spd.y += vel.y;

	// Prevent from gaining a ridiculous amount of downwards momentum
	if (spd.y > 4)
		spd.y = 4;

	updatePosition();
	collide();

	// Jump
	if (game->input->wasJustPressed(SDL_SCANCODE_X))
	{
		if (vel.y == 0)
		{
			spd.y = (fullRun) ? -4.675 : -5 - (fabsf(spd.x) / 4);
			Mix_HaltChannel(1);
			Mix_PlayChannel(1, game->jumpSound, 0);
		}
	}

	animate();

	// Get the sprite to display in the proper vector position
	imgX = curAnim.frames[floor(curFrame)];
}

void Mario::animate()
{
	if (isCrouching)
		curAnim = crouchAnim;
	else if (vel.y == 0)
	{
		if (isSkidding)
			curAnim = skidAnim;
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
				{
					curFrame = 1;
				}
				else
				{
					curFrame = 0;
				}
			}
		}
	}
	else
	{
		if (!fullRun)
			curAnim = jumpAnim;
		else
			curAnim = runJumpAnim;
		curFrame = (spd.y <= 0) ? 0 : 1;
	}

	int frameVectorSize = curAnim.frames.size();
	if (frameVectorSize <= 1)
	{
		curFrame = 0;
	}
	else if (curFrame >= frameVectorSize - 1)
	{
		if (frameVectorSize > 1)
		{
			while (curFrame >= frameVectorSize)
				curFrame -= frameVectorSize;
		}
		else
		{
			curFrame = 0;
		}
	}
	if (curFrame < 0)
		curFrame = 0;
}