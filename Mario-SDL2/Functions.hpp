#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>
#include <string>
#include <SDL.h>

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
void drawText(int x, int y, std::string msg/*, align alignStyle*/);
void trueMouseCoordinates(SDL_Renderer* renderer, SDL_Window* window, int* logicalMouseX, int* logicalMouseY);

#endif