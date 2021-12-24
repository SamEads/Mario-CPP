#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Functions.hpp"

class Game;
class Mario;
class Entity;

class Level
{
	private:
	public:
		Level(Game* game);
		void draw();
		void update();
		Game* game;
		Mario* mario;
		Vector2 camPos;
		std::vector<Entity*> entities;
		std::vector<std::vector<Tile*>> tiles;
		int levelWidth = 128;
		int levelHeight = 14;
		// Editing variables
		bool editorMode = false;
		bool inBlockMenu = false;
		int selectedCellX = 0;
		int selectedCellY = 0;
		int blinkBlockTimer = 0;
};

#endif