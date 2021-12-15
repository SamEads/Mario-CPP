#include "Mario.hpp"
#include "Game.hpp"
#include <iostream>

Animation curAnim;
Animation idleAnim;
Animation walkAnim;
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
	crouchAnim.frames = { 10 };
}

/*void Mario::draw()
{
}*/

void Mario::update()
{
	int fakeFloorLevel = 128 + 32;
	float accel = 0.0625;
	float decel = 0.0375;
	isSkidding = false;
	decel = (isGrounded) ? decel : decel / 2;
	float maxSpd = (game->input->isPressed(SDL_SCANCODE_Z) && !isCrouching) ? 2.5 : 1.25;
	int moveDir = game->input->isPressed(SDL_SCANCODE_RIGHT) - game->input->isPressed(SDL_SCANCODE_LEFT);
	if (isGrounded)
	{
		if (game->input->isPressed(SDL_SCANCODE_DOWN))
		{
			{
				if (moveDir == 0)
					isCrouching = true;
			}
		}
		if (!game->input->isPressed(SDL_SCANCODE_DOWN))
		{
			isCrouching = false;
		}
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_X))
	{
		if (isGrounded)
			spd.y = -4.75 - (fabsf(spd.x) / 4);
	}
	if (game->input->isPressed(SDL_SCANCODE_RIGHT) && (!isCrouching || !isGrounded))
	{
		if (!isCrouching || !isGrounded)
		{
			if (spd.x < 0)
			{
				isSkidding = true;
			}
			if (isGrounded)
				flipSpr = SDL_FLIP_NONE;
			if (spd.x < maxSpd)
				spd.x += accel;
		}
	}
	else if (game->input->isPressed(SDL_SCANCODE_LEFT) && (!isCrouching || !isGrounded)) 
	{
		if (!isCrouching || !isGrounded)
		{
			if (spd.x > 0)
			{
				isSkidding = true;
			}
			if (isGrounded)
				flipSpr = SDL_FLIP_HORIZONTAL;
			if (spd.x > -maxSpd)
				spd.x -= accel;
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

	spd.y += (!game->input->isPressed(SDL_SCANCODE_X)) ? 0.375 : 0.1875;

	if (spd.y > 4)
		spd.y = 4;

	position.x += spd.x;
	position.y += spd.y;

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
				curAnim = walkAnim;
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
		curAnim = jumpAnim;
		curFrame = (spd.y <= 0) ? 0 : 1;
	}

	int arrSize = curAnim.frames.size();
	if (curFrame > arrSize - 1)
	{
		if (arrSize > 0)
		{
			while (curFrame > arrSize)
				curFrame -= arrSize;
			if (curFrame < 0)
				curFrame = 0;
		}
		else
		{
			curFrame = 0;
		}
	}
}