#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

enum Alignment
{
	left,
	right
};

class Text
{
	private:
		static const std::string textOrder;
	public:
		std::string text;
		Alignment alignment = left;
		static void draw(int x, int y, std::string text, Alignment alignment = left);
};

#endif