#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <string>
#include <map>
#include <fmod.hpp>
#include <GL/glew.h>
#include <GL/GL.h>

struct Sound
{
	FMOD::Sound* fmodSound;
	FMOD::Channel* fmodChannel;
};
struct Texture
{
	GLuint id;
	uint16_t width, height;
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

#endif //!ASSETMANAGER_HPP