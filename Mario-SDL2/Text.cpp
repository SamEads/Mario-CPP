#include "Core.hpp"
#include "Game.hpp"
#include <iostream>
#include <string>
#include "Text.hpp"
#include "AssetManager.hpp"

const std::string Text::textOrder = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ*!.-x";

void Text::draw(int x, int y, std::string text, Alignment alignment)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	destRect.x = srcRect.x = srcRect.y = 0;
	destRect.w = destRect.h = srcRect.w = srcRect.h = 8;
	destRect.y = y;
	int j = text.length();
	for (int i = 0; i < text.length(); i++)
	{
		int rand = std::rand() % (1000 - 0 + 1);
		int position = textOrder.find(text[i]);
		int _x;
		if (alignment == right)
		{
			_x = x - (j * 8);
			j--;
		}
		else
			_x = x + (i * 8);
		srcRect.x = position * 8;
		destRect.x = _x;
		renderCopy(getTexture("font"), &srcRect, &destRect);
	}
}