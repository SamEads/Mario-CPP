#ifndef CORE_HPP
#define CORE_HPP

#include <vector>
#include <SDL.h>
#include <string>

#define DEBUG true;
#define USEFMOD true;
#define USEOPENGL true;

class Game;
class Texture;

extern Game* game;

void drawTile(int x, int y, int tileX, int tileY, bool followCam = true);
void initFunctions(Game* game);
void trueMouseCoordinates(SDL_Renderer* renderer, SDL_Window* window, int* logicalMouseX, int* logicalMouseY);
int renderCopy(Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect, SDL_RendererFlip flipSpr = SDL_FLIP_NONE);
float lerp(float a, float b, float t);
void resetView();

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

struct Rect
{
	float x, y, w, h;
	float getLeft() { return x; }
	float getRight() { return x + w; }
	float getTop() { return y; }
	float getBottom() { return y + h; }
	Vector2 getCenter()
	{
		Vector2 returnVec;
		returnVec.x = x + (w / 2);
		returnVec.y = y + (h / 2);
		return returnVec;
	}
	SDL_Rect getSDLRect() { return { (int)this->x, (int)this->y, (int)this->w, (int)this->h }; }
	float intersects(Rect rect) { return !(rect.getLeft() > getRight() || rect.getRight() < getLeft() || rect.getTop() > getBottom() || rect.getBottom() < getTop()); }
};

struct Animation
{
	std::vector<int> frames;
};

struct Tile
{
	Tile(int _cellX = -1, int _cellY = -1, int _width = 16, int _height = 16)
	{
		cellX = _cellX;
		cellY = _cellY;
		width = _width;
		height = _height;
	}
	int cellX,cellY,width,height;
};

enum TileType
{
	SOLID,
	SEMISOLID,
};

#endif