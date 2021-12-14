#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include "Functions.h"
#include "Mario.hpp"
#include "Input.hpp"

class Game
{
	private:
		// Methods
		void handleEvents();
		void update();
		void draw();
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
		// Textures
		SDL_Texture* playerBigTexture;
		Input* input;
};

#endif