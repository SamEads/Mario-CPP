#include "Mario.hpp"
#include "Game.hpp"
#include <iostream>

SDL_RendererFlip flipSpr;

Animation curAnim;
Animation idleAnim;
Animation walkAnim;
Animation skidAnim;
Animation jumpAnim;

float curFrame = 0;
float imgX = 0;
float imgY = 0;

Mario::Mario(Game* _game)
{
	game = _game;
	idleAnim.frames = { 0, 12 };
	walkAnim.frames = { 0, 1, 2, 1 };
	jumpAnim.frames = { 4 };
	skidAnim.frames = { 3 };
}

void Mario::draw()
{
	// Things to draw
	SDL_Rect* srcRect = new SDL_Rect();
	SDL_Rect* sizeRect = new SDL_Rect();
	SDL_Point* centerPoint = new SDL_Point();
	srcRect->x = floor(imgX) * 32;
	srcRect->y = floor(imgY) * 32;
	sizeRect->w = srcRect->w = 32;
	sizeRect->h = srcRect->h = 32;
	sizeRect->x = (position.x);
	sizeRect->y = (position.y);
	centerPoint->x = 16;
	centerPoint->y = 16;
	SDL_RenderCopyEx(game->renderer, game->playerBigTexture, srcRect, sizeRect, 0, NULL, flipSpr);
	delete srcRect;
	delete sizeRect;
	delete centerPoint;
}

void Mario::update()
{
	int fakeFloorLevel = 128;
	float accel = 0.0625;
	float decel = 0.0375;
	bool isSkidding = false;
	decel = (isGrounded) ? decel : decel / 2;
	float maxSpd = (game->input->isPressed(SDL_SCANCODE_Z)) ? 2.5 : 1.25;
	if (game->input->wasJustPressed(SDL_SCANCODE_X))
	{
		if (position.y >= fakeFloorLevel)
			spd.y = -4.25 - (fabsf(spd.x) / 4);
	}
	if (game->input->isPressed(SDL_SCANCODE_RIGHT))
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
	else if (game->input->isPressed(SDL_SCANCODE_LEFT))
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
	if (position.y >= fakeFloorLevel)
	{
		isGrounded = true;
	}
	else
	{
		isGrounded = false;
	}

	if (isGrounded)
	{
		position.y = fakeFloorLevel;
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
		curFrame = 0;
	}

	int arrSize = curAnim.frames.size();
	std::cout << curAnim.frames.size() << std::endl;
	if (curFrame > arrSize-1)
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
	int displayFrame = floor(curFrame);
	// std::cout << "UH " << arrSize << std::endl;
	imgX = curAnim.frames.at(curFrame);
}