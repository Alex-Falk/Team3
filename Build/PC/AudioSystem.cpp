#include "AudioSystem.h"

//constructor, initialises all variables automatically
AudioSystem::AudioSystem() {
	FMOD::System_Create(&audioSystem);
	int driverCount = 0;
	audioSystem->getNumDrivers(&driverCount);
	if (driverCount == 0) {
		//warn user as they will hear no sounds
		std::cout << "No Audio Driver Detected!" << std::endl;
	}

	masterVolume = 1.0f;
	gameSoundsVolume = 1.0f;
	musicVolume = 1.0f;

	audioSystem->init(numSounds, FMOD_INIT_NORMAL, NULL);
	audioSystem->set3DSettings(1.0f, 1.0f, 1.0f);

	audioSystem->createChannelGroup("GameSoundsGroup", &gameSoundsGroup);
	audioSystem->createChannelGroup("MusicGroup", &musicGroup);

	audioSystem->getMasterChannelGroup(&masterGroup);
	masterGroup->addGroup(gameSoundsGroup);
	masterGroup->addGroup(musicGroup);

	musicGroup->setVolume(masterVolume * musicVolume);
	gameSoundsGroup->setVolume(masterVolume * gameSoundsVolume);
}

AudioSystem::~AudioSystem() {
	for (int i = 0; i < numSounds; i++) {
			sounds[i]->release();
	}
	audioSystem->close();
	audioSystem->release();
}

//adds a sound to the system. For smaller files
void AudioSystem::Create2DSound(int index, const char* pFile) {
	if (sounds[index] == NULL) {
		audioSystem->createSound(pFile, FMOD_2D, 0, &sounds[index]);
	}
	else {
		sounds[index]->release();
		audioSystem->createSound(pFile, FMOD_2D, 0, &sounds[index]);
	}
}

//makes the sound a stream. For larger files that may take a while to load
void AudioSystem::Create2DStream(int index, const char* pFile) {
	if (sounds[index] == NULL) {
		audioSystem->createStream(pFile, FMOD_2D, 0, &sounds[index]);
	}
	else {
		sounds[index]->release();
		audioSystem->createStream(pFile, FMOD_2D, 0, &sounds[index]);
	}
}

//plays a certain sound, works for both sounds and streams
void AudioSystem::PlaySound(int index, bool loop) {
	if (!loop)
		sounds[index]->setMode(FMOD_LOOP_OFF);
	else
	{
		sounds[index]->setMode(FMOD_LOOP_NORMAL);
		sounds[index]->setLoopCount(-1);
	}

	audioSystem->playSound(sounds[index], 0, false, &channels[index]);
	if (index <= GAME_MUSIC) {
		channels[index]->setChannelGroup(musicGroup);
	}
	else channels[index]->setChannelGroup(gameSoundsGroup);
}

//memory management, will delete the sound
void AudioSystem::ReleaseSound(int index) {
	sounds[index]->release();
}

//temp update
void AudioSystem::Update() {
	audioSystem->update();
}

//call each frame to update the audiosystem and pass in camera parameters
void AudioSystem::Update(FMOD_VECTOR cameraPos, FMOD_VECTOR cameraForward, FMOD_VECTOR cameraUp, FMOD_VECTOR cameraVelocity) {
	listenerPos = cameraPos;
	listenerForward = cameraForward;
	listenerUp = cameraUp;
	listenerVelocity = cameraVelocity;
	audioSystem->set3DListenerAttributes(0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp);
	audioSystem->update();
}

//stop a specific sound, not pause
void AudioSystem::StopSound(int index) {
	channels[index]->stop();
}

//stop all sounds, not pause
void AudioSystem::StopAllSounds() {
	for (int i = 0; i < numSounds;i++) {
		FMOD::Channel* pChannel = nullptr;
		FMOD_RESULT res = audioSystem->getChannel(i, &pChannel);

		if (res == FMOD_OK && pChannel)
		{
			pChannel->stop();
		}
	}
}

//sets the master volume
void AudioSystem::SetMasterVolume(float f) {
	masterVolume = f;
	masterGroup->setVolume(masterVolume);
}

//sets all game sounds volume
void AudioSystem::SetGameSoundsVolume(float f) {
	gameSoundsVolume = f;
	gameSoundsGroup->setVolume(gameSoundsVolume);
}

//sets all music volume
void AudioSystem::SetMusicVolume(float f) {
	musicVolume = f;
	musicGroup->setVolume(musicVolume);
}

//sets the volume of an individual sound (overridden if group volume is called after)
void AudioSystem::SetVolume(float vol, int index) {
	channels[index]->setVolume(vol);
}

//pause all game sounds in the game
void AudioSystem::PauseGameSounds() {
	for (int i = 0; i < numSounds; i++) {
		gameSoundsGroup->setPaused(1);
	}
}

//unpause all game sounds in the game
void AudioSystem::UnpauseGameSounds() {
	for (int i = 0; i < numSounds; i++) {
		gameSoundsGroup->setPaused(0);
	}
}

//pause all music in the game
void AudioSystem::PauseMusic() {
	for (int i = 0; i < numSounds; i++) {
		musicGroup->setPaused(1);
	}
}

//unpause all music in the game
void AudioSystem::UnpauseMusic() {
	for (int i = 0; i < numSounds; i++) {
		musicGroup->setPaused(0);
	}
}

void AudioSystem::MuteAllSounds() {
	masterGroup->setMute(1);
}

void AudioSystem::UnmuteAllSounds() {
	masterGroup->setMute(0);
}
