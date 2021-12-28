#include "HUD.hpp"
#include "Level.hpp"
#include "Game.hpp"
#include "SDL.h"
#include "Text.hpp"

HUD::HUD(Level *_level)
{
	level = _level;
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
	float pmeterMax = level->mario->pmeterMax;
	float pmeterLevel = pmeterMax - level->mario->pmeterLevel;
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
}