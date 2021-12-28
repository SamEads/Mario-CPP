#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

class Game;

class Text
{
	private:
	public:
		Game* game;
		std::string text;
		void draw(int x, int y);
};

#endif