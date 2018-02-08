#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include "GamePlay.h"

//Michael Davis - 07/02/2018

class AudioSystem {
public:
	//constructor, initialises all variables automatically
	AudioSystem();

	//adds a sound to the system. For smaller files
	void Create2DSound(int index, const char* pFile);
	void Create3DSound(int index, const char* pFile, float minDist, float maxDist); //minDist is when attenuation starts, maxDist is when sound is inaudible 

	//makes the sound a stream. For larger files that may take a while to load
	//can't have multiple streams in 1 channel, mainly for music
	void Create2DStream(int index, const char* pFile);
	void Create3DStream(int index, const char* pFile, float minDist, float maxDist);

	//plays a certain sound, works for both sounds and streams
	void PlaySound(int index, bool loop = false);

	//memory management, will delete the sound
	void ReleaseSound(int index);

	//pause all game sounds in the game
	void PauseGameSounds();
	void UnpauseGameSounds();

	//pause all music in the game
	void PauseMusic();
	void UnpauseMusic();

	//stops all sounds, not pauses
	void StopAllSounds();
	void StopSound(int index);

	//sounds continue to play but muted
	void MuteAllSounds();
	void UnmuteAllSounds();

	//set volumes
	void SetMasterVolume(float f);
	void SetGameSoundsVolume(float f);
	void SetMusicVolume(float f);
	void SetVolume(float vol, int index); //sets the volume of an individual sound (overridden if group volume is called after)

	//call each frame to update the audiosystem and pass in camera parameters
	void Update(FMOD_VECTOR cameraPos, FMOD_VECTOR cameraForward, FMOD_VECTOR cameraUp, FMOD_VECTOR cameraVelocity);

	//temp update
	void Update();

	//destructor
	~AudioSystem();

private:
	//amount of sounds in the game
	const int numSounds = 5;

	//system that handles sounds and channels
	FMOD::System * audioSystem;

	//the actual sounds, includes music as well
	FMOD::Sound * sounds[5];

	//channel the sound plays from
	FMOD::Channel * channels[5];

	//camera variables for 3d sounds
	FMOD_VECTOR listenerPos;
	FMOD_VECTOR listenerForward; //normalised
	FMOD_VECTOR listenerUp; //normalised 
	FMOD_VECTOR listenerVelocity;

	//volume levels, can be controlled in options menu
	float masterVolume;
	float musicVolume;
	float gameSoundsVolume;

	//allows us to set volume levels for all sounds at once
	FMOD::ChannelGroup * gameSoundsGroup;
	FMOD::ChannelGroup * musicGroup;
	FMOD::ChannelGroup * masterGroup;
};