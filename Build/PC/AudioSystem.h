#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <fstream>
#include "GamePlay.h"
#include "../nclgl/Vector3.h"
#include "../nclgl/TSingleton.h"
#include "../nclgl/common.h"
#include "../ncltech/GameObject.h"

//Michael Davis - 07/02/2018

struct CustomSound {
	GameObject * object = NULL;
	bool isChannelPlaying = false;
};

class AudioSystem : public TSingleton<AudioSystem> {
	friend class TSingleton<AudioSystem>;

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
	void PlayASound(int index, bool loop = false, Vector3 position = { 0.f, 0.f, 0.f }, Vector3 velocity = { 0.f, 0.f, 0.f }, GameObject * go = NULL);

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
	void StopAllFinishedSounds();
	void StopSound(int index, int delay);

	//sounds continue to play but muted
	void MuteAllSounds();
	void UnmuteAllSounds();

	//set volumes
	void SetMasterVolume(float f);
	void SetGameSoundsVolume(float f);
	void SetMusicVolume(float f);

	//added by Jeffery, get volumes
	float GetMasterVolume() { return masterVolume; }
	float GetGameSoundsVolume() { return gameSoundsVolume; }
	float GetMusicVolume() { return musicVolume; }

	//call each frame to update the audiosystem and pass in camera parameters
	void Update(Vector3 cameraPos, Vector3 cameraForward, Vector3 cameraUp, float dt);
	void Update();

	uint GetSoundLength(int i){
		uint length = NULL;
		sounds[i]->getLength(&length, FMOD_TIMEUNIT_MS);
		return length;
	}

	//destructor
	~AudioSystem();

private:
	//amount of sounds in the game
	const static int numSounds = 13;
	const static int numChannels = 64;

	//system that handles sounds and channels
	FMOD::System * audioSystem;

	//the actual sounds, includes music as well
	FMOD::Sound * sounds[numSounds];

	//channel the sound plays from
	FMOD::Channel * channels[numChannels];

	CustomSound freeChannels[numChannels];

	//camera variables for 3d sounds
	FMOD_VECTOR listenerPos;
	FMOD_VECTOR listenerLastPos;
	FMOD_VECTOR listenerForward; //unit vector and perpendicular to the up vector
	FMOD_VECTOR listenerUp; //unit vector and perpendicular to the forward vector
	FMOD_VECTOR listenerVelocity;

	//volume levels, can be controlled in options menu
	float masterVolume;
	float musicVolume;
	float gameSoundsVolume;

	//allows us to set volume levels for all sounds at once
	FMOD::ChannelGroup * gameSoundsGroup;
	FMOD::ChannelGroup * musicGroup;
	FMOD::ChannelGroup * masterGroup;

	//converts vec3 to FMODVec
	friend FMOD_VECTOR toFMODVector(Vector3 v);

	//used for error checking, outputs can be found here:
	//https://www.fmod.org/docs/content/generated/FMOD_RESULT.html
	//0 = All good
	//18 = file not found
	//30 = invalid handle
	FMOD_RESULT result;
	std::ofstream myfile;
};