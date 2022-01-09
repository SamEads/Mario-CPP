#include "Entity.hpp"
#include "Game.hpp"
#include "Text.hpp"
#include <iostream>
#include <gme.h>
#include "AssetManager.hpp"
#include <filesystem>

Level* level;
Music_Emu* emu;

SDL_GLContext glContext;
GLuint frameBuffer;
GLuint renderTexture;
GLuint renderBuffer;

#define FREQ 48000
#define SAMPLES 2048

void callBack(void* userData, Uint8* out, int count)
{
	if (userData == NULL || out == NULL || !emu)
		return;
	if (gme_track_ended(emu))
		return;
	gme_play(emu, SAMPLES, (short*)out);
	//SDL_MixAudio(out, (Uint8*)buf, count, 0.3f * SDL_MIX_MAXVOLUME);
};

Game::Game()
{
	initCore(this);

	int init = SDL_Init(SDL_INIT_VIDEO);
	if (init == 0)
	{
		// Create window
		window = SDL_CreateWindow("Super Mario Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameWidth * 2, gameHeight * 2, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		SDL_SetWindowMinimumSize(window, gameWidth, gameHeight);

		// Initialize FMOD
		FMOD::System_Create(&fmodSystem);
		fmodSystem->init(64, FMOD_INIT_NORMAL, 0);

		// Initialize GME
		gme_open_file("Assets/Music/overworld.spc", &emu, FREQ);
		gme_start_track(emu, 0);
		static SDL_AudioSpec spec;
		spec.freq = FREQ;
		spec.format = AUDIO_S16SYS;
		spec.channels = 2;
		spec.samples = SAMPLES / (spec.channels);
		spec.userdata = emu;
		spec.callback = callBack;
		SDL_OpenAudio(&spec, 0);
		SDL_PauseAudio(false);
		gme_enable_accuracy(emu, true);
		gme_set_fade(emu, -1);

		// OpenGL attributes
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		glContext = SDL_GL_CreateContext(window);
		SDL_GL_SetSwapInterval(SDL_FALSE);
		glewInit();

		// Frame buffer
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenTextures(1, &renderTexture);
		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, game->gameWidth, game->gameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, game->gameWidth, game->gameHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		initShaders();

		isRunning = true;
		input = new Input();
		level = new Level(this);

		std::string path = "Assets/Images/tiles";
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string fileString = entry.path().string();
			if (fileString.find(".png") == std::string::npos)
				continue;
			std::string substrPath = fileString.substr(path.length()+1);
			substrPath = substrPath.substr(0, substrPath.find("."));
			getTexture("tiles/"+substrPath);
		}

		while (isRunning)
		{
			if (input->wasJustPressed(SDL_SCANCODE_2))
			{
				gme_delete(emu);
				emu = 0;
				gme_open_file("Assets/Music/overworld.spc", &emu, FREQ);
				gme_start_track(emu, 0);
			}
			if (input->wasJustPressed(SDL_SCANCODE_3))
			{
				gme_delete(emu);
				emu = 0;
				gme_open_file("Assets/Music/ddz.vgm", &emu, FREQ);
				gme_start_track(emu, 0);
			}
			if (input->wasJustPressed(SDL_SCANCODE_4))
			{
				gme_delete(emu);
				emu = 0;
				gme_open_file("Assets/Music/ghz2.vgm", &emu, FREQ);
				gme_start_track(emu, 0);
			}
			if (input->wasJustPressed(SDL_SCANCODE_5))
			{
				gme_delete(emu);
				emu = 0;
				gme_open_file("Assets/Music/title.spc", &emu, FREQ);
				gme_start_track(emu, 0);
			}
			if (level->time <= 100 && level->time > 0)
			{
				tempo = 1.2;
			}
			else
				tempo = 1;
			gme_set_tempo(emu, tempo);
			if (input->wasJustPressed(SDL_SCANCODE_T))
			{
				freeSounds();
			}
			handleEvents();
			update();
			draw();
			SDL_Delay(1000 / 60);
		}
		SDL_GL_DeleteContext(glContext);
	}

	delete input;
	delete level;

	gme_delete(emu);
	emu = 0;

	freeTextures();
	freeSounds();

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
	// Global "shine" tick for coins, bricks, q-blocks, etc.
	if (shineTick >= 1)
		shineTick += 0.125f;
	else
		shineTick += 0.025f;
	if (shineTick >= 4)
		shineTick -= 4;
}

void Game::draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, game->gameWidth, game->gameHeight);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	resetView();

	level->draw();

	resetView();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	int w, h;
	SDL_GL_GetDrawableSize(window, &w, &h);
	float wRatio = (float)h / game->gameHeight;
	if (gameWidth * wRatio < w)
	{
		float dispWidth = game->gameWidth * wRatio;
		glViewport((w - dispWidth) / 2, 0, dispWidth, h);
	}
	else
	{
		float ratio = (float)w / game->gameWidth;
		float dispHeight = game->gameHeight * ratio;
		glViewport(0, (h - dispHeight) / 2, w, dispHeight);
	}

	glBindTexture(GL_TEXTURE_2D, renderTexture);

	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex2f(0, game->gameHeight);
	glTexCoord2f(0, 1);
	glVertex2f(0, 0);
	glTexCoord2f(1, 1);
	glVertex2f(game->gameWidth, 0);
	glTexCoord2f(1, 0);
	glVertex2f(game->gameWidth, game->gameHeight);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	SDL_GL_SwapWindow(window);
}