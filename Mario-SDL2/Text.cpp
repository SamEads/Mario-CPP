#include "Game.hpp"
#include <iostream>
#include <string>
#include "Text.hpp"

void Text::draw(Game *game, int x, int y)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	destRect.x = srcRect.x = srcRect.y = 0;
	destRect.w = destRect.h = srcRect.w = srcRect.h = 8;
	destRect.y = y;
	int j = text.length();
	for (int i = 0; i < text.length(); i++)
	{
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
		SDL_RenderCopy(game->renderer, game->fontTexture, &srcRect, &destRect);
	}
}