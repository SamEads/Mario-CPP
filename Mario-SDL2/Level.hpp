#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Core.hpp"

class Game;
class HUD;
class Player;
class Entity;

class Level
{
	private:
	public:
		int time = 108;
		int timeTick = 0;
		Level(Game* game);
		void draw();
		void update();
		void saveLevel(const char* file);
		void loadLevel(const char* file);
		void sortEntityDepth();
		void fillBlankTiles();
		HUD* hud;
		Game* game;
		Player* player;
		Vector2 camPos;
		std::vector<Entity*> entities;
		std::vector<std::vector<Tile*>> tiles;
		int levelWidth = 128;
		int levelHeight = 14;
		// Editing variables
		bool editorMode = true;
		bool inBlockMenu = false;
		int selectedCellX = 0;
		int selectedCellY = 0;
		int blinkBlockTimer = 0;
		std::vector<std::vector<Tile>> arrangedTiles
		{
			{Tile(1, 0), Tile(2, 0), Tile(3, 0), Tile(4, 0), Tile(5, 0)},
			{Tile(1, 1), Tile(2, 1), Tile(3, 1), Tile(4, 1), Tile(6, 0)},
			{Tile(1, 2), Tile(2, 1), Tile(3, 2), Tile(4, 1), Tile(-1, -1)},
		};
};

#endif // !LEVEL_HPP