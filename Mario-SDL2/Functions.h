#include <vector>
#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP


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

#endif