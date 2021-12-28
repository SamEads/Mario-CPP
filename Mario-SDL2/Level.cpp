#include "Entity.hpp"
#include "Core.hpp"
#include "Game.hpp"
#include "Level.hpp"
#include "Mario.hpp"
#include <algorithm>
#include "Enemy.hpp"
#include "HUD.hpp"
#include <fstream>
#include "Text.hpp"

Level::Level(Game* _game)
{
	// Assign game
	game = _game;
	// Create player
	mario = new Mario(this);
	mario->position.x = 32;
	mario->position.y = 160;
	entities.push_back(mario);
	// Assign camera position
	camPos.x = 0;
	camPos.y = 0;
	Enemy *throwaway = new Enemy(this);
	throwaway->position.x = 160;
	throwaway->position.y = 160;
	entities.push_back(throwaway);
	// Load level
	std::ifstream f("Assets/Levels/level.sav");
	if (f.good())
	{
		loadLevel("Assets/Levels/level.sav");
	}
	else
	{
		fillBlankTiles();
		for (int i = 0; i < levelWidth; i++)
		{
			tiles[i][13]->cellX = 2;
			tiles[i][13]->cellY = 1;
			tiles[i][12]->cellX = 2;
			tiles[i][12]->cellY = 0;
		}
	}
	// Create HUD
	hud = new HUD(this);
}

void Level::fillBlankTiles()
{
	// Fill map up with blank tiles
	for (int x = 0; x < levelWidth; x++)
	{
		std::vector<Tile*> w;
		for (int y = 0; y < levelHeight; y++)
		{
			Tile* t = new Tile();
			w.push_back(t);
		}
		tiles.push_back(w);
	}
}

void Level::saveLevel(const char* fileName)
{
	// For formatting, see a loaded or saved level file - the order goes level size, tiles, then entities
	std::ofstream file;
	file.open(fileName);
	file << levelWidth << " " << levelHeight << std::endl;
	// Fill map up with blank tiles
	for (int x = 0; x < levelWidth; x++)
	{
		std::vector<Tile*> w;
		for (int y = 0; y < levelHeight; y++)
		{
			Tile* t = new Tile();
			w.push_back(t);
		}
		tiles.push_back(w);
	}
	for (int i = 0; i < levelHeight; i++)
	{
		for (int j = 0; j < levelWidth; j++)
		{

			file << "t" << tiles[j][i]->cellX << "," << tiles[j][i]->cellY;
			if (j < levelWidth - 1) file << " ";
		}
		file << std::endl;
	}
	for (Entity* entity : entities)
	{
		file << "[" << entity->identifier << "," << entity->position.x << "," << entity->position.y << "]";
	}
	file.close();
}

void Level::loadLevel(const char* fileName)
{
	std::ifstream file;
	file.open(fileName);
	std::string line;
	int x = 0;
	int y = 0;
	bool tileLine = false;
	bool recievedLevelSize = false;
	while (std::getline(file, line))
	{
		// Get level width & height
		if (!recievedLevelSize)
		{
			// Level size strings to convert to ints
			std::string lWS;
			std::string lHS;
			bool recievedWidth = false;
			for (int i = 0; i < line.length(); i++)
			{
				// Get width
				if (!recievedWidth)
				{
					if (line[i] != ' ')
					{
						lWS += line[i];
					}
					else
						recievedWidth = true;
				}
				// Get height
				else
				{
					if (line[i] != ' ')
						lHS += line[i];
				}
			}
			// Recieve true width & height, fill the level to the proportions
			levelWidth = atoi(lWS.c_str());
			levelHeight = atoi(lHS.c_str());
			fillBlankTiles();
			recievedLevelSize = true;
		}
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
			y++;
		}
	}
	file.close();
}

void Level::update()
{
	#if DEBUG
	if (game->input->wasJustPressed(SDL_SCANCODE_S))
	{
		saveLevel("Assets/Levels/level.sav");
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_F))
	{
		loadLevel("Assets/Levels/level.sav");
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_E) && editorMode)
	{
		inBlockMenu = !inBlockMenu;
	}
	if (game->input->wasJustPressed(SDL_SCANCODE_SPACE))
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
		if (selectedCellY / 16 < arrangedTiles.size())
		{
			if (selectedCellX / 16 < arrangedTiles[selectedCellY / 16].size())
			{
				Tile *retrievedTile = &arrangedTiles[selectedCellY / 16][selectedCellX / 16];
				if (retrievedTile->cellX != -1 || retrievedTile->cellY != -1)
				{
					tiles[x/16][y/16]->cellX = retrievedTile->cellX;
					tiles[x/16][y/16]->cellY = retrievedTile->cellY;
				}
			}
		}
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
	#endif

	for (Entity* entity : entities)
	{
		if (entity->position.x+32 > camPos.x && entity->position.x-32 < camPos.x + game->gameWidth)
			entity->update();
	}
	int screenMiddle = (camPos.x + game->gameWidth / 2) - 8;
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

Vector2 tilePositionOffset;
void Level::draw()
{
	tilePositionOffset.x = 16;
	tilePositionOffset.y = 16;
	#if DEBUG
	int x, y;
	trueMouseCoordinates(game->renderer, game->window, &x, &y);
	int cellX = ceil((x / 16) * 16);
	int cellY = ceil((y / 16) * 16);
	if (inBlockMenu && editorMode)
	{
		if (game->input->wasJustPressed(0))
		{
			selectedCellX = cellX - tilePositionOffset.x;
			selectedCellY = cellY - tilePositionOffset.y;
		}
		int blinkedDist = (blinkBlockTimer >= 20) ? 1 : 0;
		blinkBlockTimer++;
		if (blinkBlockTimer > 40)
		{
			blinkBlockTimer = 0;
		}
		/*
		SDL_Rect fullSizeImg;
		fullSizeImg.x = fullSizeImg.y = 0;
		fullSizeImg.w = 240;
		fullSizeImg.h = 144;
		SDL_RenderCopy(game->renderer, game->tilesTexture, &fullSizeImg, &fullSizeImg);
		*/
		// TODO: flip
		for (int _x = 0; _x < arrangedTiles.size(); _x++)
		{
			for (int _y = 0; _y < arrangedTiles[_x].size(); _y++)
			{
				drawTile((_y * 16) + tilePositionOffset.x, (_x * 16) + tilePositionOffset.y, arrangedTiles[_x][_y].cellX, arrangedTiles[_x][_y].cellY, false);
				//SDL_RenderCopy(game->renderer, game->tilesTexture, &tileWRect, &tileWDestRect);
			}
		}

		SDL_Rect hoveredBlock;
		SDL_Rect selectedBlock;
		hoveredBlock.x = cellX;
		hoveredBlock.y = cellY;
		hoveredBlock.w = 16;
		hoveredBlock.h = 16;
		selectedBlock.x = selectedCellX + blinkedDist + tilePositionOffset.x;
		selectedBlock.y = selectedCellY + blinkedDist + tilePositionOffset.y;
		selectedBlock.w = hoveredBlock.w - (blinkedDist * 2);
		selectedBlock.h = hoveredBlock.h - (blinkedDist * 2);
		// Draw overlay on selected block
		SDL_SetRenderDrawColor(game->renderer, 70, 0, 200, 255);
		SDL_RenderDrawRect(game->renderer, &selectedBlock);
		// Draw overlay on hovered block
		SDL_SetRenderDrawColor(game->renderer, 70, 0, 255, 128);
		SDL_RenderFillRect(game->renderer, &hoveredBlock);
		// Reset draw color
		SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
		return;
	}
	#endif

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
	// Render clouds
	SDL_RenderCopy(game->renderer, game->cloudsTexture, &cloudSourceRect, &cloudSizeRect);
	// Render tiles in region
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

	#if DEBUG
	if (!inBlockMenu && editorMode)
	{
		SDL_SetTextureAlphaMod(game->tilesTexture, 128);
		trueMouseCoordinates(game->renderer, game->window, &x, &y);
		int floorX = floor((x + camPos.x) / 16);
		int floorY = floor((y / 16) * 16) / 16;
		int drawCellX, drawCellY;
		if (selectedCellY / 16 < arrangedTiles.size())
		{
			if (selectedCellX / 16 < arrangedTiles[selectedCellY / 16].size())
			{
				Tile* retrievedTile = &arrangedTiles[selectedCellY / 16][selectedCellX / 16];
				if (retrievedTile->cellX != -1 || retrievedTile->cellY != -1)
				{
					drawCellX = retrievedTile->cellX;
					drawCellY = retrievedTile->cellY;
					drawTile(floorX * 16, floorY * 16, drawCellX, drawCellY);
				}
			}
		}
		SDL_SetTextureAlphaMod(game->tilesTexture, 255);
	}
	#endif

	// Draw every entity
	for (Entity* entity : entities)
	{
		entity->draw(entity->texture, game, entity->position.x, entity->position.y);
	}

	// Draw heads up display
	hud->draw();

	Text text;
	text.alignment = left;
	text.text = (!mario->luigi) ? "MARIO" : "LUIGI";
	text.draw(game, 24, 16);
	text.text = "000000";
	text.draw(game, 24, 24);
	text.text = "x00";
	text.draw(game, 96, 24);
	/*text.text = "x00";
	text.draw(game, 32, 24);*/
	text.text = "WORLD";
	text.draw(game, 144, 16);
	text.text = "!-!";
	text.draw(game, 152, 24);
	text.alignment = right;
	text.text = "TIME";
	text.draw(game, game->gameWidth - 24, 16);
	text.text = "000";
	text.draw(game, game->gameWidth - 24, 24);
	SDL_Rect coinSrcRect, coinDstRect;
	auto shineTick = game->shineTick >= 3 ? 5 : (floor(game->shineTick) * 5);
	coinSrcRect.x = 44 + shineTick;
	coinSrcRect.y = 0;
	coinDstRect.w = coinSrcRect.w = 5;
	coinDstRect.h = coinSrcRect.h = 8;
	coinDstRect.x = 90;
	coinDstRect.y = 24;
	SDL_RenderCopy(game->renderer, game->hudTextures, &coinSrcRect, &coinDstRect);
}