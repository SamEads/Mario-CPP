#include "Functions.hpp"
#include "Entity.hpp"
#include "Mario.hpp"
#include "Game.hpp"
#include "Level.hpp"

Level::Level(Game* _game)
{
	game = _game;
	mario = new Mario(game);
	entities.push_back(mario);
}

bool inBlockMenu = false;

void Level::update()
{
	for (Entity* entity : entities)
	{
		entity->update();
	}
	camPos.x = mario->position.x - game->gameWidth / 2 + 8;
	camPos.y = mario->position.y - game->gameHeight / 2;
	if (camPos.x < 0)
	{
		camPos.x = 0;
	}
	int camBoundsX = levelWidth * 16;
	if (camPos.x + game->gameWidth > camBoundsX)
	{
		camPos.x = camBoundsX - game->gameWidth;
	}

	if (game->input->wasJustPressed(SDL_SCANCODE_SPACE))
	{
		inBlockMenu = !inBlockMenu;
	}
}

void Level::draw()
{

	if (inBlockMenu)
	{
		SDL_Rect fullSizeImg;
		fullSizeImg.x = fullSizeImg.y = 0;
		fullSizeImg.w = 256;
		fullSizeImg.h = 224;
		SDL_RenderCopy(game->renderer, game->tilesTexture, &fullSizeImg, &fullSizeImg);
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

	/*for (int i = 0; i < 4; i++)
	{
		drawTile(128, 176 - (i * 16), 5, 0);
	}*/

	for (Entity* entity : entities)
	{
		entity->draw(game->playerBigTexture, game, entity->position.x, entity->position.y);
	}
}