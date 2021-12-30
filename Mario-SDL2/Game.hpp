#ifndef GAME_HPP
#define GAME_HPP

#include "InputManager.hpp"
#include "Core.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <fmod.hpp>

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
		SDL_Texture* hillsTexture;
		SDL_Texture* fontTexture;
		SDL_Texture* hudTextures;
		FMOD::System* fmodSystem;

		Input* input;
		Level* level;
		const int gameWidth = 256;
		const int gameHeight = 224;
		float shineTick = 0;
		float tempo = 1;

		int coins = 6;
		int score = 100;
};

#endif