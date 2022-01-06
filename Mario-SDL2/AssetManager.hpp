#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <string>
#include <map>
#include <fmod.hpp>
#include <GL/glew.h>
#include <SDL_opengl.h>

struct Mix_Chunk;
struct SDL_Texture;
struct Sound
{
	FMOD::Sound* fmodSound;
	FMOD::Channel* fmodChannel;
	Mix_Chunk* mixChunk;
	int mixChannel;
};
struct Texture
{
	SDL_Texture* sdlTexture;
	GLuint id;
	int width, height;
};

/* Play a sound or get a texture, if the sound/texture is not yet loaded then it will be loaded into their corresponding map */
void playSound(std::string soundDirectory, bool loops = false, bool stopPrevious = false, float volume = 1.0f);
Texture* getTexture(std::string textureDirectory);
int createShader(const std::string& vertexShader, const std::string& fragmentShader);
static unsigned int compileShader(unsigned int type, const std::string& source);

/* Free sounds or textures from memory */
void freeTextures();
void freeSounds();

extern std::map<std::string, Sound*> sounds;
extern std::map<std::string, Texture*> textures;
extern int soundChannels;

#endif