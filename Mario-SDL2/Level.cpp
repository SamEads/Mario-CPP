#include "Entity.hpp"
#include "Core.hpp"
#include "Game.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include <algorithm>
#include "Enemy.hpp"
#include "HUD.hpp"
#include <fstream>
#include "Goomba.hpp"
#include "Text.hpp"
#include "AssetManager.hpp"

Vector2 mousePosition;

Level::Level(Game* _game)
{
	// Assign game
	game = _game;
	// Create player
	player = new Player(this);
	player->position.x = 32;
	player->position.y = 160;
	entities.push_back(player);
	// Assign camera position
	camPos.x = 0;
	camPos.y = 0;
	entities.reserve(5000);
	for (int i = 0; i < 2; i++)
	{
		Goomba *throwaway = new Goomba(this);
		throwaway->position.x = 128+(i*24);
		throwaway->position.y = 160;
		entities.push_back(throwaway);
	}
	// Load level
	std::ifstream f("Assets/Levels/level.sav");
	levelWidth = 1024;
	if (1 == 2)//if (f.good())
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
	sortEntityDepth();
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

float cameraOffset = 0;
int camXDir = 0;
bool camAdjusting = false;
void Level::update()
{
	int mpX, mpY;
	trueMouseCoordinates(&mpX, &mpY);
	mousePosition.x = mpX;
	mousePosition.y = mpY;
	int entityCountStart = entities.size();
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
		if (editorMode)
		{
			loadLevel("Assets/Levels/level.sav");
		}
	}
#endif
	if (inBlockMenu)
		return;
	else if (editorMode)
	{
		if (game->input->isPressed(0))
		{
			int x, y;
			trueMouseCoordinates(&x, &y);
			x = floor(((x + camPos.x) / 16) * 16);
			y = floor((y / 16) * 16);
			if (selectedCellY / 16 < arrangedTiles.size())
			{
				if (selectedCellX / 16 < arrangedTiles[selectedCellY / 16].size())
				{
					Tile* retrievedTile = &arrangedTiles[selectedCellY / 16][selectedCellX / 16];
					if (retrievedTile->cellX != -1 || retrievedTile->cellY != -1)
					{
						tiles[x / 16][y / 16]->cellX = retrievedTile->cellX;
						tiles[x / 16][y / 16]->cellY = retrievedTile->cellY;
					}
				}
			}
		}
		else if (game->input->isPressed(1))
		{
			int x, y;
			trueMouseCoordinates(&x, &y);
			x = floor(((x + camPos.x) / 16) * 16);
			y = floor((y / 16) * 16);
			tiles[x / 16][y / 16]->cellX = -1;
			tiles[x / 16][y / 16]->cellY = -1;
		}
	}

	if (!editorMode)
	{
		for (Entity* entity : entities)
		{
			if (entity->position.x + 32 > camPos.x && entity->position.x - 32 < camPos.x + game->gameWidth)
				entity->update();
		}
		if (cameraOffset < game->gameWidth / 2)
		{
			cameraOffset = game->gameWidth / 2;
		}
		int innerBuffer = 8;
		int outerBuffer = 32;
		if (player->getHitbox().getCenter().x > cameraOffset + outerBuffer)
		{
			camXDir = 1;
		}
		else if (player->getHitbox().getCenter().x < cameraOffset - outerBuffer)
		{
			camXDir = -1;
		}
		if (camXDir == 1)
		{
			if (cameraOffset < player->getHitbox().getCenter().x + innerBuffer)
			{
				cameraOffset += 2;
				if (player->spd.x > 0)
					cameraOffset += player->spd.x;
				if (cameraOffset >= player->getHitbox().getCenter().x + innerBuffer)
					cameraOffset = player->getHitbox().getCenter().x + innerBuffer;
			}
		}
		else if (camXDir == -1)
		{
			if (cameraOffset > player->getHitbox().getCenter().x - innerBuffer)
			{
				cameraOffset -= 2;
				if (player->spd.x < 0)
					cameraOffset += player->spd.x;
				if (cameraOffset <= player->getHitbox().getCenter().x - innerBuffer)
					cameraOffset = player->getHitbox().getCenter().x - innerBuffer;
			}
		}
		camPos.x = cameraOffset - (game->gameWidth / 2);
		timeTick++;
		if (timeTick >= 40)
		{
			timeTick = 0;
			if (time > 0)
			{
				time--;
				if (time == 100)
				{
					playSound("hurry", false, false, 1.0f);
				}
			}
		}
	}
	else
	{
		if (game->input->isPressed(SDL_SCANCODE_D))
		{
			camPos.x += 4;
		}
		else if (game->input->isPressed(SDL_SCANCODE_A))
		{
			camPos.x -= 4;
		}
	}
	camPos.y = player->position.y - game->gameHeight / 2;
	camPos.x = clamp(camPos.x, 0, (levelWidth * 16) - game->gameWidth);
	// DEPTH SORTING
	if (entityCountStart != entities.size())
	{
		sortEntityDepth();
	}
}

void Level::sortEntityDepth()
{
	std::sort(entities.begin(), entities.end(), [](Entity* a, Entity* b)
	{
		return a->depth > b->depth;
	});
}

Vector2 tilePositionOffset;
void Level::draw()
{
	glTranslatef(ceil(camPos.x) * -1, 0 * -1, 0);
	tilePositionOffset.x = 16;
	tilePositionOffset.y = 16;
	// Cut-out of the texture
	Rect cloudSourceRect;
	Rect cloudSizeRect;
	cloudSourceRect.x = ceil(camPos.x / 2);
	cloudSourceRect.y = 0;
	cloudSourceRect.w = game->gameWidth;
	cloudSourceRect.h = game->gameHeight;
	cloudSizeRect.x = ceil(camPos.x);
	cloudSizeRect.y = 0;
	cloudSizeRect.w = game->gameWidth;
	cloudSizeRect.h = game->gameHeight;
	// Render clouds
	renderCopy(getTexture("bg/clouds_bg"), &cloudSourceRect, &cloudSizeRect);
	// Render tiles in region
	int curLevelWFocus = ceil(camPos.x / 16) + (game->gameWidth + 16) / 16;
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
		glColor4f(1, 1, 1, 0.5f);
		int floorX = floor((mousePosition.x + camPos.x) / 16);
		int floorY = floor((mousePosition.y / 16) * 16) / 16;
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
		glColor4f(1, 1, 1, 1);
	}
	#endif

	// Draw every entity
	for (Entity* entity : entities)
	{
		entity->draw();
	}

	hud->draw();

#if DEBUG
	int cellX = ceil(((int)mousePosition.x / 16) * 16);
	int cellY = ceil(((int)mousePosition.y / 16) * 16);
	if (inBlockMenu && editorMode)
	{
		resetView();
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor4f(0, 0, 0.2f, 0.8f);
		glRecti(0, 0, game->gameWidth, game->gameHeight);
		if (game->input->wasJustPressed(0))
		{
			selectedCellX = cellX - tilePositionOffset.x;
			selectedCellY = cellY - tilePositionOffset.y;
		}
		int blinkedDist = 0;
		if (blinkBlockTimer > 40)
		{
			blinkBlockTimer = 0;
		}
		// TODO: flip
		glColor4f(1, 1, 1, 1);

		for (int _x = 0; _x < arrangedTiles.size(); _x++)
		{
			for (int _y = 0; _y < arrangedTiles[_x].size(); _y++)
			{
				if (selectedCellX/16 == _y && selectedCellY/16 == _x)
					glColor4f(1, 1, 1, 1);
				else
					glColor4f(0.7, 0.7, 0.9, 0.8);
				drawTile((_y * 16) + tilePositionOffset.x, (_x * 16) + tilePositionOffset.y, arrangedTiles[_x][_y].cellX, arrangedTiles[_x][_y].cellY, false);
			}
		}

		glColor4f(1, 1, 1, 1);

		Recti hoveredBlock;
		Recti selectedBlock;
		hoveredBlock.x = cellX;
		hoveredBlock.y = cellY;
		hoveredBlock.w = 16;
		hoveredBlock.h = 16;
		selectedBlock.x = selectedCellX + blinkedDist + tilePositionOffset.x;
		selectedBlock.y = selectedCellY + blinkedDist + tilePositionOffset.y;
		selectedBlock.w = hoveredBlock.w - (blinkedDist * 2);
		selectedBlock.h = hoveredBlock.h - (blinkedDist * 2);
		// Draw overlay on selected block
		glColor4f(70.0f / 255.0f, 0, 200.0f / 255.0f, 1);
		glBindTexture(GL_TEXTURE_2D, 0);
		/*glBegin(GL_LINE_LOOP);
		glVertex2i(selectedBlock.x, selectedBlock.y);
		glVertex2i(selectedBlock.x + selectedBlock.w, selectedBlock.y);
		glVertex2i(selectedBlock.x + selectedBlock.w, selectedBlock.y + selectedBlock.h);
		glVertex2i(selectedBlock.x, selectedBlock.y + selectedBlock.h);
		glEnd();*/
		// Draw overlay on hovered block
		glColor4f(70.0f / 255.0f, 0, 1.0f, 128.0f / 255.0f);
		glRecti(hoveredBlock.x, hoveredBlock.y, hoveredBlock.x + hoveredBlock.w, hoveredBlock.y + hoveredBlock.h);
		// Reset draw color
		glColor4f(1, 1, 1, 1);
		return;
	}
#endif
}