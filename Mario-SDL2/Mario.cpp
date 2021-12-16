#include "Mario.hpp"
#include "Game.hpp"
#include <iostream>

Animation curAnim;
Animation idleAnim;
Animation walkAnim;
Animation runAnim;
Animation runJumpAnim;
Animation skidAnim;
Animation jumpAnim;
Animation crouchAnim;
float curFrame = 0;

Mario::Mario(Game* _game)
{
	game = _game;
	idleAnim.frames = { 0, 12 };
	walkAnim.frames = { 0, 1, 2, 1 };
	jumpAnim.frames = { 4, 4 };
	skidAnim.frames = { 3 };
	runAnim.frames = { 6, 7, 8, 7 };
	runJumpAnim.frames = { 9 };
	crouchAnim.frames = { 10 };
}

/*void Mario::draw()
{
}*/

bool fullRun = false;
void Mario::update()
{
	int fakeFloorLevel = 128 + 32;
	float accel = 0.0625;
	float decel = 0.0375;
	isSkidding = false;
	decel = (isGrounded) ? decel : decel / 2;
	float maxSpd = (game->input->isPressed(SDL_SCANCODE_Z) && !isCrouching) ? (fullRun) ? 3 : 2.5 : 1.25;
	int moveDir = game->input->isPressed(SDL_SCANCODE_RIGHT) - game->input->isPressed(SDL_SCANCODE_LEFT);
	if (maxSpd >= 2.5 && fabsf(spd.x) > 1.25 && moveDir != 0)
	{
		if (isGrounded)
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
		if (isGrounded)
			fullRun = false;
		if (pmeterLevel < pmeterMax)
			pmeterLevel++;
		else
			pmeterLevel = pmeterMax;
	}
	if (isGrounded)
	{
		if (game->input->isPressed(SDL_SCANCODE_DOWN))
		{
			{
				isCrouching = true;
			}
		}
		if (!game->input->isPressed(SDL_SCANCODE_DOWN))
		{
			isCrouching = false;
		}
	}
	if (moveDir != 0 && (!isCrouching || !isGrounded))
	{
		if (!isCrouching || !isGrounded)
		{
			if (spd.x * moveDir < 0)
			{
				isSkidding = true;
			}
			if (isGrounded)
				flipSpr = (moveDir < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
			if (spd.x * moveDir < maxSpd)
				spd.x += accel * moveDir;
		}
	}
	else if (spd.x > 0)
	{
		spd.x -= decel;
		if (spd.x <= 0)
			spd.x = 0;
	}
	else if (spd.x < 0)
	{
		spd.x += decel;
		if (spd.x >= 0)
			spd.x = 0;
	}
	if (spd.x >= maxSpd)
	{
		spd.x -= decel;
		if (spd.x < maxSpd)
			spd.x = maxSpd;
	}
	else if (spd.x <= -maxSpd)
	{
		spd.x += decel;
		if (spd.x > -maxSpd)
			spd.x = -maxSpd;
	}

	if (!fullRun)
		spd.y += (!game->input->isPressed(SDL_SCANCODE_X)) ? 0.375 : 0.1875;
	else
		spd.y += (!game->input->isPressed(SDL_SCANCODE_X)) ? 0.375 : 0.125;

	if (spd.y > 4)
		spd.y = 4;

	if (game->input->wasJustPressed(SDL_SCANCODE_X))
	{
		if (isGrounded)
		{
			if (fullRun)
				spd.y = -5;
			else
				spd.y = -4.875 - (fabsf(spd.x) / 4);
		}
	}

	updatePosition();
	animate();

	// If under solid
	if (position.y >= fakeFloorLevel && spd.y >= 0)
	{
		isGrounded = true;
		position.y = fakeFloorLevel;
	}

	// If not under solid
	if (position.y < fakeFloorLevel)
	{
		isGrounded = false;
	}

	int displayFrame = floor(curFrame);
	imgX = curAnim.frames.at(curFrame);
}

void Mario::animate()
{
	if (isCrouching)
	{
		curAnim = crouchAnim;
		curFrame = 0;
	}
	else if (isGrounded)
	{
		if (isSkidding)
		{
			curAnim = skidAnim;
			curFrame = 0;
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

	int arrSize = curAnim.frames.size();
	if (arrSize <= 1)
	{
		curFrame = 0;
	}
	else if (curFrame > arrSize - 1)
	{
		if (arrSize > 1)
		{
			while (curFrame > arrSize)
				curFrame -= arrSize;
		}
		else
		{
			curFrame = 0;
		}
	}
	if (curFrame < 0)
		curFrame = 0;
	std::cout << curFrame << std::endl;
}