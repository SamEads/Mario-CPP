#include "Core.hpp"
#include "Game.hpp"
#include <iostream>
#include <SDL_mixer.h>
#include "SoundManager.hpp"

std::map<std::string, Sound*> sounds;
int soundChannels = 0;

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