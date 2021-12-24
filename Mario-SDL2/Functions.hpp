#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>
#include <string>
#include <SDL.h>

class Game;

static Game* game;

/*
Better rectangle "struct" (class) - needed float values for precise collisions!!!
*/
class Rect
{
	public:
		float x;
		float y;
		float w;
		float h;
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
};

struct Animation
{
	std::vector<int> frames;
};

struct Tile
{
	int cellX;
	int cellY;
};

void drawTile(int x, int y, int tileX, int tileY);
void initFunctions(Game* game);
void drawText(int x, int y, std::string msg/*, align alignStyle*/);
void trueMouseCoordinates(SDL_Renderer* renderer, SDL_Window* window, int* logicalMouseX, int* logicalMouseY);

#endif