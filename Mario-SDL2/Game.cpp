#include "Entity.hpp"
#include "Game.hpp"
#include "Text.hpp"
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <gme.h>

Level* level;
Music_Emu* emu;
SDL_Texture* buffer;

#define FREQ 48000
#define SAMPLES 2048

SDL_Texture* Game::loadImage(std::string location)
{
	SDL_Surface* tempSurface = IMG_Load(location.c_str());
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}

void audioCallback(void* userData, Uint8* out, int count)
{
	if (userData == NULL || out == NULL || !emu || gme_track_ended(emu))
		return;
	gme_play(emu, SAMPLES, (short*) out);
}

Game::Game()
{
	initFunctions(this);
	std::cout << "Initializing" << std::endl;
	init = SDL_Init(SDL_INIT_VIDEO);
	if (init == 0)
	{
		if (Mix_OpenAudio(FREQ, AUDIO_S16SYS, 1, SAMPLES) >= 0)
			std::cout << "Initialized audio system" << std::endl;
		FMOD::System_Create(&fmodSystem);
		fmodSystem->init(64, FMOD_INIT_NORMAL, 0);
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
			buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, gameWidth, gameHeight);
			if (renderer)
			{
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_RenderSetLogicalSize(renderer, gameWidth, gameHeight);
				//SDL_RenderSetScale(renderer, 1, 1);
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
				isRunning = true;
				std::cout << "Entering main loop" << std::endl;

				marioTexture = loadImage("Assets/Images/mario.png");
				luigiTexture = loadImage("Assets/Images/luigi.png");
				foesTexture = loadImage("Assets/Images/foes.png");
				tilesTexture = loadImage("Assets/Images/tiles.png");
				cloudsTexture = loadImage("Assets/Images/clouds_bg.png");
				hillsTexture = loadImage("Assets/Images/tiles.png");
				fontTexture = loadImage("Assets/Images/font.png");
				hudTextures = loadImage("Assets/Images/hud.png");

				input = new Input();
				level = new Level(this);

				gme_open_file("Assets/Sounds/overworld.spc", &emu, FREQ);
				gme_start_track(emu, 0);
				static SDL_AudioSpec spec;
				spec.freq = FREQ;
				spec.format = AUDIO_S16SYS;
				spec.channels = 2;
				spec.samples = SAMPLES / (spec.channels);
				spec.callback = audioCallback;
				spec.userdata = emu;
				SDL_OpenAudio(&spec, 0);
				SDL_PauseAudio(false);
				gme_enable_accuracy(emu, true);
				// LOOP MUSIC FOREVER
				gme_set_fade(emu, -1);
				while (isRunning)
				{
					if (input->wasJustPressed(SDL_SCANCODE_2))
					{
						gme_open_file("Assets/Sounds/overworld.spc", &emu, FREQ);
						gme_start_track(emu, 0);
					}
					if (input->wasJustPressed(SDL_SCANCODE_3))
					{
						//gme_open_file("Assets/Sounds/athletic.spc", &emu, FREQ);
						//gme_start_track(emu, 0);
					}
					if (input->wasJustPressed(SDL_SCANCODE_3))
					{
						gme_open_file("Assets/Sounds/snow.spc", &emu, FREQ);
						gme_start_track(emu, 0);
					}
					if (input->wasJustPressed(SDL_SCANCODE_4))
					{
						gme_open_file("Assets/Sounds/map.spc", &emu, FREQ);
						gme_start_track(emu, 0);
					}
					if (input->wasJustPressed(SDL_SCANCODE_5))
					{
						gme_open_file("Assets/Sounds/title.spc", &emu, FREQ);
						gme_start_track(emu, 0);
					}
					if (input->wasJustPressed(SDL_SCANCODE_O))
					{
						tempo += 0.1f;
						gme_set_tempo(emu, tempo);
					}
					if (input->wasJustPressed(SDL_SCANCODE_U))
					{
						tempo -= 0.1f;
						gme_set_tempo(emu, tempo);
					}
					if (input->wasJustPressed(SDL_SCANCODE_I))
					{
						tempo = 1;
						gme_set_tempo(emu, tempo);
					}
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

	gme_delete(emu);
	emu = 0;
	SDL_DestroyTexture(marioTexture);
	SDL_DestroyTexture(luigiTexture);
	SDL_DestroyTexture(foesTexture);
	SDL_DestroyTexture(tilesTexture);
	SDL_DestroyTexture(fontTexture);
	SDL_DestroyTexture(hudTextures);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
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
	fmodSystem->update();
	if (shineTick >= 1)
		shineTick += 0.125f;
	else
		shineTick += 0.025f;
	if (shineTick >= 4)
		shineTick -= 4;
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

	SDL_SetRenderTarget(renderer, buffer);

	// Prepare a white background, clip the background, and draw everything
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderSetClipRect(renderer, &rect);
	level->draw();

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, buffer, NULL, NULL);

	// Draw to window
	SDL_RenderPresent(renderer);
}