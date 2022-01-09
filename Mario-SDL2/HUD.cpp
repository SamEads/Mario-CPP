#include "HUD.hpp"
#include "Level.hpp"
#include "Game.hpp"
#include "SDL.h"
#include "Text.hpp"
#include "AssetManager.hpp"
#include <GL/GL.h>

Texture* hudTexture;

HUD::HUD(Level *_level)
{
	level = _level;
	hudTexture = getTexture("ui/hud");
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

int lastHUDScore = 0;
float lerpScore = 0;
void HUD::draw()
{
	// Reset view
	resetView();
	if (level->editorMode)
	{
		Text::draw(game->gameWidth - 8, 8, "LEVEL EDITOR MODE", RIGHT);
		Text::draw(game->gameWidth - 8, 16, "PRESS E TO OPEN ITEMS LIST", RIGHT);
		return;
	}

	// HUD element cut-out
	Rect sourceRect;
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = 7;
	sourceRect.h = 8;
	// HUD element position & stretch
	Rect destRect;
	destRect.x = 32;
	destRect.y = game->gameHeight - sourceRect.h - 12;
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
		renderCopy(hudTexture, &sourceRect, &destRect);
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
	renderCopy(hudTexture, &sourceRect, &destRect);
	Text::draw(24, 16, (level->player->character == MARIO) ? "MARIO" : "LUIGI");
	if (abs(game->score - lerpScore) > 0.5f)
		lerpScore = lerp(lerpScore, game->score, 0.5f);
	else
		lerpScore = game->score;
	Text::draw(24, 24, addLeadingZeroes(std::to_string((int) lerpScore), 6));
	Text::draw((game->gameWidth / 2) - (game->gameWidth / 8), 24, "x" + addLeadingZeroes(std::to_string(game->coins), 2));
	Text::draw((game->gameWidth/2) + (game->gameWidth / 8) - 16, 16, "WORLD");
	Text::draw((game->gameWidth / 2) + (game->gameWidth / 8) - 8, 24, "!-!");
	Text::draw(game->gameWidth - 24, 16, "TIME", RIGHT);
	Text::draw(game->gameWidth - 24, 24, addLeadingZeroes(std::to_string(level->time), 3), RIGHT);
	Rect coinSrcRect, coinDstRect;
	float shineTick = floor(game->shineTick) * 5;
	coinSrcRect.x = 44 + shineTick;
	coinSrcRect.y = 0;
	coinDstRect.w = coinSrcRect.w = 5;
	coinDstRect.h = coinSrcRect.h = 8;
	coinDstRect.x = (game->gameWidth / 2) - (game->gameWidth / 8) - 8;
	coinDstRect.y = 24;
	renderCopy(hudTexture, &coinSrcRect, &coinDstRect);
	lastHUDScore = game->score;
}