#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <string>
#include <map>
#include <fmod.hpp>

struct Mix_Chunk;
struct Sound
{
	FMOD::Sound* fmodSound;
	FMOD::Channel* fmodChannel;
	Mix_Chunk* mixChunk;
	int mixChannel;
};

void playSound(std::string soundDirectory, bool loops = false, bool stopPrevious = false, float volume = 1.0f);

extern std::map<std::string, Sound*> sounds;
extern int soundChannels;

#endif