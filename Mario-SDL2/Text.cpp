#include <string>
#include "Text.hpp"
#include <iostream>
#include "Game.hpp"

void Text::draw(Game *game, int x, int y)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 8;
	srcRect.h = 8;
	destRect.x = 0;
	destRect.y = y;
	destRect.w = 8;
	destRect.h = 8;
	int j = text.length();
	for (int i = 0; i < text.length(); i++)
	{
		int position = textOrder.find(text[i]);
		int _x = x + (i * 8);
		if (alignment == right)
		{
			_x = x - (j * 8);
			j--;
		}
		srcRect.x = position * 8;
		destRect.x = _x;
		SDL_RenderCopy(game->renderer, game->fontTexture, &srcRect, &destRect);
	}
}