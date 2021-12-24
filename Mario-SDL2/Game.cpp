#include "Entity.hpp"
#include "Game.hpp"
#include <iostream>
#include <windows.h>
#include <GL/gl.h>

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
	init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (init == 0)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0)
			std::cout << "Initialized audio system" << std::endl;
		std::cout << "Creating window" << std::endl;
		window = SDL_CreateWindow("Super Mario Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameWidth * 2, gameHeight * 2, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GLContext mainContext = SDL_GL_CreateContext(window);
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

				marioTexture = loadImage("Assets/Images/mario.png");
				luigiTexture = loadImage("Assets/Images/luigi.png");
				foesTexture = loadImage("Assets/Images/foes.png");
				tilesTexture = loadImage("Assets/Images/tiles.png");
				cloudsTexture = loadImage("Assets/Images/clouds_bg.png");

				input = new Input();
				level = new Level(this);

				jumpSound = Mix_LoadWAV("Assets/Sounds/jump.wav");

				/*Music_Emu* emu = 0;
				SDL_AudioSpec spec;
				gme_open_file("Assets/Sounds/ow.spc", &emu, 48000);
				gme_start_track(emu, 0);
				short buf[1024];
				if (!gme_play(emu, 1024, buf))
					std::cout << "GME PLAYING" << std::endl;
				spec.freq = 48000;
				spec.format = AUDIO_S16;
				spec.channels = 2;
				spec.samples = 512;
				spec.callback = gme_feedaudio;
				spec.userdata = NULL;
				SDL_OpenAudio(&spec, 0);
				*/

				while (isRunning)
				{
					handleEvents();
					update();
					draw();
					SDL_Delay(1000 / 60);
				}
				std::cout << "Exited main loop" << std::endl;
				SDL_GL_DeleteContext(mainContext);
			}
		}
	}

	delete input;

	SDL_DestroyTexture(marioTexture);
	SDL_DestroyTexture(luigiTexture);
	SDL_DestroyTexture(foesTexture);
	SDL_DestroyTexture(tilesTexture);

	TTF_CloseFont(font);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	// Get current mouse buttons
	memcpy(input->lastMouseButtons, input->curMouseButtons, 2);

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
	/*
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);
	*/
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