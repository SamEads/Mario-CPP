#ifndef GAME_HPP
#define GAME_HPP

#include "Input.hpp"
#include "Functions.hpp"
#include "Mario.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Level.hpp"

class Camera;

class Game
{
	private:
		// Methods
		void draw();
		void handleEvents();
		void update();
		SDL_Texture* loadImage(std::string location);
		// Variables
		bool isRunning;
		int init;
	public:
		// Constructor
		Game();
		// Variables
		SDL_Renderer* renderer;
		SDL_Window* window;
		TTF_Font* font;
		SDL_Texture* playerBigTexture;
		SDL_Texture* tilesTexture;
		SDL_Texture* cloudsTexture;
		Input* input;
		int gameWidth = 256;
		int gameHeight = 224;
		Level* level;
};

#endif