#include "Core.hpp"
#include "Game.hpp"
#include <iostream>
#include "AssetManager.hpp"
#include <SDL_image.h>

std::map<std::string, Sound*> sounds;
std::map<std::string, Texture*> textures;
int soundChannels = 0;

GLuint compileShader(unsigned int type, const std::string& source)
{
	// create shader & get c string from c++ string
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();

	// compile shader
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// error info
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == false)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "FAILED TO COMPILE " << ((type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << "SHADER" << std::endl;
		glDeleteShader(id);
		return 0;
	}

	// compile & return shader id
	std::cout << "COMPILED " << ((type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << " SHADER" << std::endl;
	return id;
}

int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//glAttachShader(program, vs);
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
		glDeleteTextures(1, &it->second->id);
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
		FMOD::Sound* playSound;
		game->fmodSystem->createSound(("Assets/Sounds/" + soundDirectory + ".wav").c_str(), FMOD_DEFAULT, 0, &playSound);
		sound = new Sound();
		sound->fmodSound = playSound;
		FMOD::Channel* ch = 0;
		sound->fmodChannel = ch;
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
		unsigned int soundLength;
		unsigned int soundPosition;
		sound->fmodChannel->isPlaying(&isPlaying);
		sound->fmodSound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
		sound->fmodChannel->getPosition(&soundPosition, FMOD_TIMEUNIT_MS);
		if (soundPosition >= soundLength)
			isPlaying = false;
		if (isPlaying)
			return;
	}
	if (stopPrevious)
		sound->fmodChannel->stop();
	game->fmodSystem->playSound(sound->fmodSound, 0, false, &sound->fmodChannel);
}