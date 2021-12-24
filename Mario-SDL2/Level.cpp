#include "Entity.hpp"
#include "Functions.hpp"
#include "Game.hpp"
#include "Level.hpp"
#include "Mario.hpp"
#include <algorithm>
#include "Enemy.hpp"
#include <fstream>

Level::Level(Game* _game)
{
	game = _game;
	mario = new Mario(this);
	mario->position.x = 32;
	mario->position.y = 160;
	entities.push_back(mario);
	Mario* luigi = new Mario(this);
	luigi->position.x = 0;
	luigi->position.y = 160;
	luigi->luigi = true;
	entities.push_back(luigi);
	camPos.x = 0;
	camPos.y = 0;
	Enemy *throwaway = new Enemy(this);
	entities.push_back(throwaway);
	// Fill map up with blank tiles
	for (int x = 0; x < levelWidth; x++)
	{
		std::vector<Tile*> w;
		for (int y = 0; y < levelHeight; y++)
		{
			Tile *t = new Tile();
			t->cellX = -1;
			t->cellY = -1;
			w.push_back(t);
		}
		tiles.push_back(w);
	}
	// Make floor
	for (int i = 0; i < levelWidth; i++)
	{
		tiles[i][13]->cellX = 2;
		tiles[i][13]->cellY = 1;
		tiles[i][12]->cellX = 2;
		tiles[i][12]->cellY = 0;
	}
}

void Level::update()
{
	if (game->input->wasJustPressed(SDL_SCANCODE_S))
	{
		std::ofstream file;
		file.open("level.sav");
		file << levelWidth << " " << levelHeight << std::endl;
		for (int i = 0; i < levelHeight; i++)
		{
			for (int j = 0; j < levelWidth; j++)
			{

				file << "t" << tiles[j][i]->cellX << "," << tiles[j][i]->cellY;
				if (j < levelWidth-1) file << " ";
			}
			file << std::endl;
		}
		for (Entity* entity : entities)
		{
			file << "[" << entity->identifier << "," << entity->position.x << "," << entity->position.y << "]";
		}
		file.close();

	}
	if (game->input->wasJustPressed(SDL_SCANCODE_F))
	{
		std::ifstream file;
		file.open("level.sav");
		std::string line;
		int x = 0;
		int y = 0;
		bool tileLine = false;
		while (std::getline(file, line))
		{
			tileLine = false;
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == 't')
				{
					tileLine = true;
					int cellX, cellY;
					i++;
					std::string strCellX, strCellY;
					while (line[i] != ',')
					{
						strCellX += line[i];
						i++;
					}
					i++;
					while (line[i] != ' ' && i < line.length())
					{
						strCellY += line[i];
						i++;
					}
					cellX = atoi(strCellX.c_str());
					cellY = atoi(strCellY.c_str());
					tiles[x][y]->cellX = cellX;
					tiles[x][y]->cellY = cellY;
					x++;
				}
			}
			if (tileLine)
			{
				x = 0;
				y ++;
			}
		}
		file.close();
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_SPACE) && editorMode)
	{
		inBlockMenu = !inBlockMenu;
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_E))
	{
		editorMode = !editorMode;
	}
	if (inBlockMenu)
		return;
	else if (game->input->isPressed(0) && editorMode)
	{
		int x, y;
		trueMouseCoordinates(game->renderer, game->window, &x, &y);
		x = floor(((x + camPos.x) / 16) * 16);
		y = floor((y / 16) * 16);
		tiles[x/16][y/16]->cellX = selectedCellX / 16;
		tiles[x/16][y/16]->cellY = selectedCellY / 16;
	}
	else if (game->input->isPressed(1) && editorMode)
	{
		int x, y;
		trueMouseCoordinates(game->renderer, game->window, &x, &y);
		x = floor(((x + camPos.x) / 16) * 16);
		y = floor((y / 16) * 16);
		tiles[x / 16][y / 16]->cellX = -1;
		tiles[x / 16][y / 16]->cellY = -1;
	}
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

void Level::draw()
{
	int x, y;
	trueMouseCoordinates(game->renderer, game->window, &x, &y);
	int cellX = ceil((x / 16) * 16);
	int cellY = ceil((y / 16) * 16);
	if (inBlockMenu && editorMode)
	{
		if (game->input->wasJustPressed(0))
		{
			selectedCellX = cellX;
			selectedCellY = cellY;
		}
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
		// Draw overlay on selected block
		SDL_SetRenderDrawColor(game->renderer, 70, 0, 200, 255);
		SDL_RenderDrawRect(game->renderer, &selectedBlock);
		// Draw overlay on hovered block
		SDL_SetRenderDrawColor(game->renderer, 70, 0, 255, 128);
		SDL_RenderFillRect(game->renderer, &hoveredBlock);

		SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
		return;
	}
	// Cut-out of the texture
	SDL_Rect cloudSourceRect;
	SDL_Rect cloudSizeRect;
	cloudSourceRect.x = camPos.x / 2;
	cloudSourceRect.y = 0;
	cloudSourceRect.w = game->gameWidth;
	cloudSourceRect.h = game->gameHeight;
	cloudSizeRect.x = 0;
	cloudSizeRect.y = 0;
	cloudSizeRect.w = game->gameWidth;
	cloudSizeRect.h = game->gameHeight;
	SDL_RenderCopy(game->renderer, game->cloudsTexture, &cloudSourceRect, &cloudSizeRect);
	int curLevelWFocus = ceil(camPos.x / 16) + game->gameWidth / 16;
	for (int x = floor(camPos.x / 16); x < std::min(curLevelWFocus, levelWidth); x++)
	{
		for (int y = 0; y < levelHeight; y++)
		{
			if (tiles[x][y]->cellX != -1)
			{
				drawTile(x * 16, y * 16, tiles[x][y]->cellX, tiles[x][y]->cellY);
			}
		}
	}
	if (!inBlockMenu && editorMode)
	{
		SDL_SetTextureAlphaMod(game->tilesTexture, 128);
		trueMouseCoordinates(game->renderer, game->window, &x, &y);
		int floorX = floor((x + camPos.x) / 16);
		int floorY = floor((y / 16) * 16) / 16;
		drawTile(floorX * 16, floorY * 16, selectedCellX / 16, selectedCellY / 16);
		SDL_SetTextureAlphaMod(game->tilesTexture, 255);
	}
	for (Entity* entity : entities)
	{
		entity->draw(entity->texture, game, entity->position.x, entity->position.y);
	}
}