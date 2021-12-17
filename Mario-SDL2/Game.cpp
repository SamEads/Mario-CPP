#include "Entity.hpp"
#include "Game.hpp"
#include <SDL_image.h>
#include <iostream>

Level* level;

SDL_Texture* Game::loadImage(std::string location)
{
	SDL_Surface* tempSurface = IMG_Load(location.c_str());
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}

Game::Game()
{
	initFunctions(this);
	std::cout << "Initializing" << std::endl;
	init = SDL_Init(SDL_INIT_VIDEO);
	if (init == 0)
	{
		std::cout << "Creating window" << std::endl;
		window = SDL_CreateWindow("Super Mario Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameWidth * 3, gameHeight * 3, SDL_WINDOW_RESIZABLE);
		SDL_SetWindowMinimumSize(window, gameWidth, gameHeight);
		if (window)
		{
			std::cout << "Creating renderer" << std::endl;
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer)
			{
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_RenderSetLogicalSize(renderer, gameWidth, gameHeight);
				isRunning = true;
				std::cout << "Initializing font system" << std::endl;
				int ttf = TTF_Init();
				if (ttf == 0)
					font = TTF_OpenFont("Assets/font.ttf", 8);
				std::cout << "Entering main loop" << std::endl;

				playerBigTexture = loadImage("Assets/Images/mario_big.png");
				tilesTexture = loadImage("Assets/Images/tiles.png");
				cloudsTexture = loadImage("Assets/Images/clouds_bg.png");

				input = new Input();
				level = new Level(this);

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

	// Free textures
	SDL_DestroyTexture(playerBigTexture);
	SDL_DestroyTexture(tilesTexture);
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

	// Get current mouse buttons
	memcpy(input->curMouseButtons, input->lastMouseButtons, 2);

	switch (event.type)
	{
		default:
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
				case (SDL_BUTTON_LEFT):
					input->curMouseButtons[0] = 1;
					break;
				case (SDL_BUTTON_RIGHT):
					input->curMouseButtons[1] = 1;
					break;
				default:
					break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
				case (SDL_BUTTON_LEFT):
					input->curMouseButtons[0] = 0;
					break;
				case (SDL_BUTTON_RIGHT):
					input->curMouseButtons[1] = 0;
					break;
				default:
					break;
			}
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
	}
};

void Game::update()
{
	level->update();
	input->update();
}

void Game::draw()
{
	// Set up draw
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw inner bounds protective background
	SDL_Rect rect;
	rect.y = rect.x = 0;
	rect.w = gameWidth;
	rect.h = gameHeight;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);

	// Prepare a white background
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	level->draw();

	// Draw
	SDL_RenderPresent(renderer);
}