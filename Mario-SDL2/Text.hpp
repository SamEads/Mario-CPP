#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

enum Alignment
{
	LEFT,
	RIGHT
};

class Text
{
	private:
		static const std::string textOrder;
	public:
		std::string text;
		Alignment alignment = LEFT;
		static void draw(int x, int y, std::string text, Alignment alignment = LEFT);
};

#endif // !TEXT_HPP