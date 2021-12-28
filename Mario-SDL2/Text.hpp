#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

class Game;

enum Alignment
{
	left,
	right
};

class Text
{
	private:
		const std::string textOrder = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ*!.-x";
	public:
		Game* game;
		std::string text;
		Alignment alignment = left;
		void draw(Game *game, int x, int y);
};

#endif