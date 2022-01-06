#ifndef GAME_HPP
#define GAME_HPP

#include "InputManager.hpp"
#include "Core.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include <fmod.hpp>
#include <SDL.h>
#include <SDL_image.h>
#if !USEFMOD
#include <SDL_mixer.h>
#endif

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
		FMOD::System* fmodSystem;

		Input* input;
		Level* level;
		SDL_Texture* buffer;
		const int gameWidth = 256;
		const int gameHeight = 224;
		float shineTick = 0;
		float tempo = 1;

		int coins = 6;
		int score = 100;
};

#endif