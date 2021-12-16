#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>

class Game;

static Game* game;

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
	uint16_t x;
	uint16_t y;
};

void drawTile(int x, int y, int tileX, int tileY);
void initFunctions(Game* game);

#endif