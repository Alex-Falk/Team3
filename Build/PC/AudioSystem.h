#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>

//Michael Davis - 07/02/2018

class AudioSystem {
public:

	//constructor
	AudioSystem() {
		FMOD::System_Create(&audioSystem);
		int driverCount = 0;
		audioSystem->getNumDrivers(&driverCount);
		if (driverCount == 0) {
			//warn user as they will hear no sounds
			std::cout << "No Audio Driver Detected!" << std::endl;
		}
		audioSystem->init(numSounds, FMOD_INIT_NORMAL, NULL);
		audioSystem->set3DSettings(1.0f, 1.0f, 1.0f);
	}

	//adds a sound to the system. Do this at the start for all sounds
	void createSound(int index, const char* pFile) {
		audioSystem->createSound(pFile, FMOD_DEFAULT, 0, &sounds[index]);
	}

	//plays a certain sound
	void playSound(int index, bool loop = false) {
		if (!loop)
			sounds[index]->setMode(FMOD_LOOP_OFF);
		else
		{
			sounds[index]->setMode(FMOD_LOOP_NORMAL);
			sounds[index]->setLoopCount(-1);
		}

		audioSystem->playSound(sounds[index], 0, false, &channels[index]);
	}

	//memory management, will delete the sound
	void releaseSound(int index) {
		sounds[index]->release();

	}

	//sets the volume of an individual sound
	void SetVolume(float vol, int index) {
		channels[index]->setVolume(vol);
	}

	//completely stops a sound
	void stopSound(int index) {
		channels[index]->stop();
	}

	//stops all sounds currently playing
	void stopAllSounds() {
		for (int i = 0; i < numSounds;i++) {
			FMOD::Channel* pChannel = nullptr;
			FMOD_RESULT res = audioSystem->getChannel(i, &pChannel);

			if (res == FMOD_OK && pChannel)
			{
				pChannel->stop();
			}
		}
	}

	//call each frame to update the audiosystem and pass in new playerposition
	void Update(FMOD_VECTOR cameraPos, FMOD_VECTOR cameraForward, FMOD_VECTOR cameraUp, FMOD_VECTOR cameraVelocity) {
		listenerPos = cameraPos;
		listenerForward = cameraForward;
		listenerUp = cameraUp;
		listenerVelocity = cameraVelocity;
		audioSystem->set3DListenerAttributes(0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp);
		audioSystem->update();
	}

	//temp update
	void Update() {
		audioSystem->update();
	}

	//destructor
	~AudioSystem() {
		for (int i = 0; i < numSounds; i++) {
			if (sounds[i] != NULL)
				sounds[i]->release();
		}
		audioSystem->close();
		audioSystem->release();
	}

private:
	//amount of sounds in the game
	const int numSounds = 5;

	//system that handles sounds and channels
	FMOD::System * audioSystem;

	//the actual sounds, includes music as well
	FMOD::Sound * sounds[5];

	//channel the sound plays from
	FMOD::Channel * channels[5];

	//camera position for 3d sounds
	FMOD_VECTOR listenerPos;
	FMOD_VECTOR listenerForward;
	FMOD_VECTOR listenerUp;
	FMOD_VECTOR listenerVelocity;

	float masterVolume;
	float musicVolume;
	float gameSoundVolume;
};