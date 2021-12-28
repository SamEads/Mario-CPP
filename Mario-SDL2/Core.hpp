#ifndef CORE_HPP
#define CORE_HPP

#include <vector>
#include <SDL.h>
#include <string>

#define DEBUG true;

class Game;

static Game* game;

void drawTile(int x, int y, int tileX, int tileY, bool followCam = true);
void initFunctions(Game* game);
void trueMouseCoordinates(SDL_Renderer* renderer, SDL_Window* window, int* logicalMouseX, int* logicalMouseY);

struct Rect
{
	float x;
	float y;
	float w;
	float h;
	SDL_Rect rect;
	SDL_Rect SDL_Rect()
	{
		rect.x = (int) this->x;
		rect.y = (int) this->y;
		rect.w = (int) this->w;
		rect.h = (int) this->h;
		return rect;
	}
	float getLeft()
	{
		return x;
	}
	float getRight()
	{
		return x + w;
	}
	float getTop()
	{
		return y;
	}
	float getBottom()
	{
		return y + h;
	}
	float intersects(Rect rect)
	{
		return !(rect.getLeft() > getRight() || rect.getRight() < getLeft() || rect.getTop() > getBottom() || rect.getBottom() < getTop());
	}
};

struct Vector2
{
	float x;
	float y;
	void operator += (Vector2 &v2)
	{
		this->x += v2.x;
		this->y += v2.y;
	}
	void operator -= (Vector2& v2)
	{
		this->x -= v2.x;
		this->y -= v2.y;
	}
};

struct Animation
{
	std::vector<int> frames;
};

struct Tile
{
	Tile()
	{
		cellX = -1;
		cellY = -1;
	}
	Tile(int _cellX, int _cellY)
	{
		cellX = _cellX;
		cellY = _cellY;
	}
	int cellX;
	int cellY;
};

#endif