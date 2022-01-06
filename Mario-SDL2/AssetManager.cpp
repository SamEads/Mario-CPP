#include "Core.hpp"
#include "Game.hpp"
#include <iostream>
#if !USEFMOD
#include <SDL_mixer.h>
#endif
#include "AssetManager.hpp"
#include <SDL_image.h>

std::map<std::string, Sound*> sounds;
std::map<std::string, Texture*> textures;
int soundChannels = 0;

unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(GL_VERTEX_SHADER);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	return id;
}

int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

Texture* getTexture(std::string textureDirectory)
{
	auto textureIt = textures.find(textureDirectory);
	// If texture does not exist in map, add it
	if (textureIt == textures.end())
	{
		SDL_Surface* tempSurface = IMG_Load(("Assets/Images/" + textureDirectory + ".png").c_str());
		Texture* emplaceTex = new Texture();
#if !USEOPENGL
		SDL_Texture* tex = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
		emplaceTex->sdlTexture = tex;
#else
		GLuint glTexID;
		int mode;
		glGenTextures(1, &glTexID);
		glBindTexture(GL_TEXTURE_2D, glTexID);
		mode = (tempSurface->format->BitsPerPixel >= 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, mode, tempSurface->w, tempSurface->h, 0, mode, GL_UNSIGNED_BYTE, tempSurface->pixels);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		emplaceTex->id = glTexID;
#endif
		textures.emplace(textureDirectory, emplaceTex);
		emplaceTex->width = tempSurface->w;
		emplaceTex->height = tempSurface->h;
		SDL_FreeSurface(tempSurface);
		return emplaceTex;
	}
	else
	{
		return textures.find(textureDirectory)->second;
	}
	return NULL;
}

void freeTextures()
{
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		SDL_Texture* tex = it->second->sdlTexture;
		SDL_DestroyTexture(tex);
	}
	textures = {};
}

void freeSounds()
{
	for (auto it = sounds.begin(); it != sounds.end(); it++)
	{
		Sound* sound = it->second;
		delete sound;
	}
	sounds = {};
	soundChannels = 0;
}

void playSound(std::string soundDirectory, bool loops, bool stopPrevious, float volume)
{
	auto soundIt = sounds.find(soundDirectory);
	Sound* sound = new Sound();
	// If sound does not exist in map, add it
	if (soundIt == sounds.end())
	{
#if USEFMOD
		FMOD::Sound* playSound;
		game->fmodSystem->createSound(("Assets/Sounds/" + soundDirectory + ".wav").c_str(), FMOD_DEFAULT, 0, &playSound);
		sound = new Sound();
		sound->fmodSound = playSound;
		FMOD::Channel* ch = 0;
		sound->fmodChannel = ch;
#else
		Mix_Chunk* playSound = Mix_LoadWAV(("Assets/Sounds/" + soundDirectory + ".wav").c_str());
		sound = new Sound();
		sound->mixChunk = playSound;
		sound->mixChannel = soundChannels;
		sounds.emplace(soundDirectory, sound);
		soundChannels++;
#endif
		sounds.emplace(soundDirectory, sound);
		soundChannels++;
	}
	else
	{
		sound = sounds.find(soundDirectory)->second;
	}
	if (loops)
	{
		bool isPlaying;
#if USEFMOD
		unsigned int soundLength;
		unsigned int soundPosition;
		sound->fmodChannel->isPlaying(&isPlaying);
		sound->fmodSound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
		sound->fmodChannel->getPosition(&soundPosition, FMOD_TIMEUNIT_MS);
		if (soundPosition >= soundLength)
			isPlaying = false;
#else
		isPlaying = Mix_Playing(sound->mixChannel);
#endif
		if (isPlaying)
			return;
	}
#if USEFMOD
	if (stopPrevious)
		sound->fmodChannel->stop();
	game->fmodSystem->playSound(sound->fmodSound, 0, false, &sound->fmodChannel);
#else
	if (stopPrevious)
		Mix_HaltChannel(sound->mixChannel);
	Mix_PlayChannel(sound->mixChannel, sound->mixChunk, loops);
#endif
}