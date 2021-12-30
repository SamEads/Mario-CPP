#include "HUD.hpp"
#include "Level.hpp"
#include "Game.hpp"
#include "SDL.h"
#include "Text.hpp"

HUD::HUD(Level *_level)
{
	level = _level;
}

std::string addLeadingZeroes(std::string str, int zeroes)
{
	int zeroesLength = str.length();
	if (zeroesLength < zeroes)
	{
		std::string strZeroes;
		zeroesLength = zeroes - zeroesLength;
		for (int i = 0; i < zeroesLength; i++)
		{
			strZeroes += "0";
		}
		str = strZeroes + str;
	}
	return str;
}

void HUD::draw()
{
	// HUD element cut-out
	SDL_Rect sourceRect;
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = 7;
	sourceRect.h = 8;
	// HUD element position & stretch
	SDL_Rect destRect;
	destRect.x = 32;
	destRect.y = level->game->gameHeight - sourceRect.h - 12;
	destRect.w = sourceRect.w;
	destRect.h = sourceRect.h;
	// P-meter bar
	float pmeterMax = level->player->pmeterMax;
	float pmeterLevel = pmeterMax - level->player->pmeterLevel;
	float pmeterRatio = (pmeterLevel / pmeterMax) * 6;
	// Render the arrows
	for (int i = 0; i < 6; i++)
	{
		sourceRect.x = (pmeterRatio <= i) ? 0 : 7;
		SDL_RenderCopy(level->game->renderer, level->game->hudTextures, &sourceRect, &destRect);
		destRect.x += 8;
	}
	// Render the p-meter symbol
	sourceRect.w = 15;
	sourceRect.x = (pFlash < 5) ? 14 : sourceRect.w + 14;
	if (pmeterLevel >= pmeterMax)
	{
		pFlash++;
		if (pFlash >= 10)
			pFlash = 0;
	}
	else
		pFlash = 0;
	destRect.w = sourceRect.w;
	SDL_RenderCopy(level->game->renderer, level->game->hudTextures, &sourceRect, &destRect);
	Text text;
	text.alignment = left;
	text.text = (level->player->character == MARIO) ? "MARIO" : "LUIGI";
	text.draw(level->game, 24, 16);
	text.text = addLeadingZeroes(std::to_string(level->game->score), 6);
	text.draw(level->game, 24, 24);
	text.text = "x"+addLeadingZeroes(std::to_string(level->game->coins), 2);
	text.draw(level->game, 96, 24);
	text.text = "WORLD";
	text.draw(level->game, 144, 16);
	text.text = "!-!";
	text.draw(level->game, 152, 24);
	text.alignment = right;
	text.text = "TIME";
	text.draw(level->game, level->game->gameWidth - 24, 16);
	text.text = addLeadingZeroes(std::to_string(level->time), 3);
	text.draw(level->game, level->game->gameWidth - 24, 24);
	SDL_Rect coinSrcRect, coinDstRect;
	float shineTick = floor(level->game->shineTick) * 5;
	coinSrcRect.x = 44 + shineTick;
	coinSrcRect.y = 0;
	coinDstRect.w = coinSrcRect.w = 5;
	coinDstRect.h = coinSrcRect.h = 8;
	coinDstRect.x = 90;
	coinDstRect.y = 24;
	SDL_RenderCopy(level->game->renderer, level->game->hudTextures, &coinSrcRect, &coinDstRect);
}