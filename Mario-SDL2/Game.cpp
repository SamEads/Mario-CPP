#include "Entity.hpp"
#include "Game.hpp"
#include "Text.hpp"
#include <iostream>
#include <gme.h>
#include "AssetManager.hpp"


Level* level;
Music_Emu* emu;

#define FREQ 48000
#define SAMPLES 2048

void audioCallback(void* userData, Uint8* out, int count)
{
	if (userData == NULL || out == NULL || !emu || gme_track_ended(emu))
		return;
	gme_play(emu, SAMPLES, (short*) out);
}

Game::Game()
{
	std::srand(std::time(nullptr));
	initFunctions(this);
	std::cout << "Initializing" << std::endl;
	init = SDL_Init(SDL_INIT_VIDEO);
	if (init == 0)
	{
		#if !USEFMOD
		if (Mix_OpenAudio(FREQ, AUDIO_S16SYS, 1, SAMPLES) >= 0)
			std::cout << "Initialized audio system" << std::endl;
		#endif
		FMOD::System_Create(&fmodSystem);
		fmodSystem->init(64, FMOD_INIT_NORMAL, 0);
		std::cout << "Creating window" << std::endl;
		window = SDL_CreateWindow("Super Mario Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameWidth * 2, gameHeight * 2, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		SDL_SetWindowMinimumSize(window, gameWidth, gameHeight);

		// OpenGL attributes
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		SDL_GLContext mainContext = SDL_GL_CreateContext(window);

		SDL_GL_SetSwapInterval(SDL_FALSE);

		if (window)
		{
			bool rendererConfirmed = false;
			#if !USEOPENGL
			std::cout << "Creating renderer" << std::endl;
			renderer = SDL_CreateRenderer(window, -1, 0);
			buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, gameWidth, gameHeight);
			if (renderer)
				rendererConfirmed = true;
			#else
			rendererConfirmed = true;
			#endif
			if (rendererConfirmed)
			{
				#if !USEOPENGL
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_RenderSetLogicalSize(renderer, gameWidth, gameHeight);
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
				#endif
				isRunning = true;
				std::cout << "Entering main loop" << std::endl;

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
						gme_open_file("Assets/Sounds/snow.spc", &emu, FREQ);
						gme_start_track(emu, 0);
					}
					if (input->wasJustPressed(SDL_SCANCODE_4))
					{
						gme_open_file("Assets/Sounds/ghz2.vgm", &emu, FREQ);
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
					if (input->wasJustPressed(SDL_SCANCODE_T))
					{
						freeSounds();
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
	freeTextures();
	freeSounds();
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
#if !USEOPENGL
	// Set up draw
	SDL_SetRenderTarget(renderer, buffer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw inner bounds protective background
	SDL_Rect rect;
	rect.y = rect.x = 0;
	rect.w = gameWidth;
	rect.h = gameHeight;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);

	// Prepare a white background, clip the background, and draw everything
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderSetClipRect(renderer, &rect);
#else
	int w, h;
	SDL_GL_GetDrawableSize(window, &w, &h);
	float ratio = (float) h / game->gameHeight;
	float dispWidth = game->gameWidth * ratio;
	glViewport((w - dispWidth) / 2, 0, dispWidth, h);
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, game->gameWidth, game->gameHeight, 0, -1, 1);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
	level->draw();
#if !USEOPENGL
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, buffer, NULL, NULL);

	// Draw to window
	SDL_RenderPresent(renderer);
#else
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	SDL_GL_SwapWindow(window);
#endif
}