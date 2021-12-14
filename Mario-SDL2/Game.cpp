#include "Game.hpp"
#include <SDL_image.h>
#include <iostream>

Mario* mario;

Game::Game()
{
	std::cout << "Initializing" << std::endl;
	init = SDL_Init(SDL_INIT_VIDEO);
	if (init == 0)
	{
		std::cout << "Creating window" << std::endl;
		window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256 * 3, 224 * 3, SDL_WINDOW_RESIZABLE);
		if (window)
		{
			std::cout << "Creating renderer" << std::endl;
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer)
			{
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_RenderSetLogicalSize(renderer, 256, 224);
				isRunning = true;
				std::cout << "Initializing font system" << std::endl;
				int ttf = TTF_Init();
				if (ttf == 0)
					font = TTF_OpenFont("Assets/Fonts/font.ttf", 8);
				std::cout << "Entering main loop" << std::endl;

				SDL_Surface* tempSurface = IMG_Load("Assets/Images/mario_big.png");
				playerBigTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
				SDL_FreeSurface(tempSurface);

				input = new Input();
				mario = new Mario(this);

				while (isRunning)
				{
					handleEvents();
					update();
					draw();
					SDL_Delay(1000 / 60);
				}
				std::cout << "Exited main loop" << std::endl;
			}
		}
	}

	delete input;
	delete mario;

	// Free textures
	SDL_DestroyTexture(playerBigTexture);
	// Quit out
	std::cout << "Closing font system" << std::endl;
	TTF_CloseFont(font);
	TTF_Quit();
	std::cout << "Closing renderer" << std::endl;
	SDL_DestroyRenderer(renderer);
	std::cout << "Closing window" << std::endl;
	SDL_DestroyWindow(window);
	std::cout << "Quitting SDL" << std::endl;
	SDL_Quit();
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		default:
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
	}
}

void Game::update()
{
	input->update();
	mario->update();
}

void Game::draw()
{
	// Set up draw
	SDL_SetRenderDrawColor(renderer, 30, 90, 80, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	mario->draw();

	// Draw
	SDL_RenderPresent(renderer);
}