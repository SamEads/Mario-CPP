#include "Entity.hpp"
#include "Functions.hpp"
#include "Game.hpp"
#include "Level.hpp"
#include "Mario.hpp"

Level::Level(Game* _game)
{
	game = _game;
	mario = new Mario(game);
	entities.push_back(mario);
	camPos.x = 0;
	camPos.y = 0;
}

bool inBlockMenu = false;
void Level::update()
{
	if (inBlockMenu)
		return;
	for (Entity* entity : entities)
	{
		entity->update();
	}
	int screenMiddle = (camPos.x + game->gameWidth / 2) - 16;
	if (mario->position.x > screenMiddle + 16)
	{
		if (mario->spd.x > 0)
			camPos.x += mario->spd.x;
	}
	else if (mario->position.x < screenMiddle - 16)
	{
		if (mario->spd.x < 0)
			camPos.x += mario->spd.x;
	}
	camPos.y = mario->position.y - game->gameHeight / 2;
	camPos.x = SDL_clamp(camPos.x, 0, (levelWidth * 16) - game->gameWidth);
}

int selectedCellX = 0;
int selectedCellY = 0;
int blinkBlockTimer = 0;
void Level::draw()
{
	int x, y;
	trueMouseCoordinates(game->renderer, game->window, &x, &y);
	int cellX = ceil((x / 16) * 16);
	int cellY = ceil((y / 16) * 16);
	if (game->input->wasJustPressed(SDL_SCANCODE_SPACE))
	{
		inBlockMenu = !inBlockMenu;
	}
	if (game->input->wasJustPressed(0))
	{
		selectedCellX = cellX;
		selectedCellY = cellY;
	}
	if (inBlockMenu)
	{
		int blinkedDist = (blinkBlockTimer >= 20) ? 1 : 0;
		blinkBlockTimer++;
		if (blinkBlockTimer > 40)
		{
			blinkBlockTimer = 0;
		}
		SDL_Rect fullSizeImg;
		fullSizeImg.x = fullSizeImg.y = 0;
		fullSizeImg.w = 240;
		fullSizeImg.h = 144;
		SDL_RenderCopy(game->renderer, game->tilesTexture, &fullSizeImg, &fullSizeImg);
		SDL_Rect hoveredBlock;
		SDL_Rect selectedBlock;
		hoveredBlock.x = cellX;
		hoveredBlock.y = cellY;
		hoveredBlock.w = 16;
		hoveredBlock.h = 16;
		selectedBlock.x = selectedCellX + blinkedDist;
		selectedBlock.y = selectedCellY + blinkedDist;
		selectedBlock.w = hoveredBlock.w - (blinkedDist * 2);
		selectedBlock.h = hoveredBlock.h - (blinkedDist * 2);
		SDL_SetRenderDrawColor(game->renderer, 70, 0, 200, 255);
		SDL_RenderDrawRect(game->renderer, &selectedBlock);
		SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 128);
		SDL_RenderFillRect(game->renderer, &hoveredBlock);
		SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
		return;
	}
	// Cut-out of the texture
	SDL_Rect cloudSourceRect;
	SDL_Rect cloudSizeRect;
	cloudSourceRect.x = camPos.x / 2;
	cloudSourceRect.y = 0;
	cloudSourceRect.w = 256;
	cloudSourceRect.h = 224;
	cloudSizeRect.x = 0;
	cloudSizeRect.y = 0;
	cloudSizeRect.w = 256;
	cloudSizeRect.h = 224;
	SDL_RenderCopy(game->renderer, game->cloudsTexture, &cloudSourceRect, &cloudSizeRect);
	for (int i = 0; i < levelWidth; i++)
	{
		drawTile(i * 16, 208, 2, 1);
		drawTile(i * 16, 192, 2, 0);
	}
	for (Entity* entity : entities)
	{
		entity->draw(game->playerBigTexture, game, entity->position.x, entity->position.y);
	}
	/*SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 128);
	SDL_Rect guiRect;
	guiRect.x = 0;
	guiRect.y = 0;
	int len = ("111111111" + std::to_string(x)).length();
	if (("111111111" + std::to_string(y)).length() > len)
	{
		len = ("111111111" + std::to_string(y)).length();
	}
	guiRect.w = len * 8;
	guiRect.h = 16;
	SDL_RenderFillRect(game->renderer, &guiRect);
	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
	drawText(0, 0, "Mouse x: " + std::to_string(x));
	drawText(0, 8, "Mouse y: " + std::to_string(y));*/
}