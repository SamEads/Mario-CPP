#ifndef GAME_HPP
#define GAME_HPP

#include "InputManager.hpp"
#include "Core.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include <fmod.hpp>
#include <SDL_image.h>

class Camera;

class Game
{
	private:
		// Methods
		void draw();
		void handleEvents();
		void update();
		// Variables
	public:
		bool isRunning;
		// Constructor
		Game();
		// Variables
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

#endif // !GAME_HPP