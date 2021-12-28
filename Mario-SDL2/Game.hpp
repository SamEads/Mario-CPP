#ifndef GAME_HPP
#define GAME_HPP

#include "Input.hpp"
#include "Core.hpp"
#include "Mario.hpp"
#include "Level.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

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
		SDL_Texture* marioTexture;
		SDL_Texture* luigiTexture;
		SDL_Texture* foesTexture;
		SDL_Texture* tilesTexture;
		SDL_Texture* cloudsTexture;
		SDL_Texture* fontTexture;
		SDL_Texture* hudTextures;

		Mix_Chunk* jumpSound;
		Mix_Chunk* pmeterSound;
		Mix_Chunk* skidSound;
		Mix_Chunk* bumpSound;

		Input* input;
		Level* level;
		const int gameWidth = 256;
		const int gameHeight = 224;
		float shineTick = 0;
		float tempo = 1;
};

#endif