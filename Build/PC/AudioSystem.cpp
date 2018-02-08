#include "AudioSystem.h"

//Michael Davis 08/02/2018

//constructor, initialises all variables automatically
AudioSystem::AudioSystem() {
	FMOD::System_Create(&audioSystem);
	int driverCount = 0;
	audioSystem->getNumDrivers(&driverCount);
	if (driverCount == 0) {
		//warn user as they will hear no sounds
		std::cout << "No Audio Driver Detected!" << std::endl;
	}

	listenerPos = { 0.0f,0.0f,0.0f };
	listenerLastPos = { 0.0f,0.0f,0.0f };
	listenerForward = { 0.0f,0.0f, 1.f };
	listenerUp = { 0.0f,1.0f,0.0f };
	listenerVelocity = { 0.0f,0.0f,0.0f };

	masterVolume = 1.0f;
	gameSoundsVolume = 1.0f;
	musicVolume = 1.0f;

	audioSystem->init(numSounds, FMOD_INIT_3D_RIGHTHANDED, NULL);
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
//TODO error checking for file path
void AudioSystem::Create2DSound(int index, const char* pFile) {
	if (sounds[index] == NULL) {
		audioSystem->createSound(pFile, FMOD_2D, 0, &sounds[index]);
	}
	else {
		sounds[index]->release();
		audioSystem->createSound(pFile, FMOD_2D, 0, &sounds[index]);
	}
}

//3D sounds, attenuation is controlled with minDist and maxDist.
void AudioSystem::Create3DSound(int index, const char * pFile, float minDist, float maxDist) {
	if (sounds[index] == NULL) {
		//TODO look into attenuation a bit more and the 3D sound types
		audioSystem->createSound(pFile, FMOD_3D_LINEARROLLOFF, 0, &sounds[index]);
		sounds[index]->set3DMinMaxDistance(minDist, maxDist);
	}
	else {
		sounds[index]->release();
		audioSystem->createSound(pFile, FMOD_3D_LINEARROLLOFF, 0, &sounds[index]);
		sounds[index]->set3DMinMaxDistance(minDist, maxDist);
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

//3D streams. May be a large file that needs to be streamed constantly in 3D space. Works the same
//as 3D sounds
void AudioSystem::Create3DStream(int index, const char* pFile, float minDist, float maxDist) {
	if (sounds[index] == NULL) {
		audioSystem->createStream(pFile, FMOD_3D, 0, &sounds[index]);
		sounds[index]->set3DMinMaxDistance(minDist, maxDist);
	}
	else {
		sounds[index]->release();
		audioSystem->createStream(pFile, FMOD_3D, 0, &sounds[index]);
		sounds[index]->set3DMinMaxDistance(minDist, maxDist);
	}
}

//plays a certain sound, works for both sounds and streams
void AudioSystem::PlaySound(int index, bool loop, FMOD_VECTOR position, FMOD_VECTOR velocity) {
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
		channels[index]->set3DAttributes(&position, &velocity);
	}
	else {
		channels[index]->setChannelGroup(gameSoundsGroup);
		channels[index]->set3DAttributes(&position, &velocity);
	}
}

//memory management, will delete the sound
void AudioSystem::ReleaseSound(int index) {
	sounds[index]->release();
}

//temp update
void AudioSystem::Update() {
	audioSystem->set3DListenerAttributes(0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp);
	audioSystem->update();
}

/*call each frame to update the audiosystem and pass in camera parameters
forward
The forwards orientation of the listener.This vector must be of unit length and perpendicular to the up vector.You can specify 0 or NULL to not update the forwards orientation of the listener.
up
The upwards orientation of the listener.This vector must be of unit length and perpendicular to the forwards vector.You can specify 0 or NULL to not update the upwards orientation of the listener.
*/
void AudioSystem::Update(Vector3 cameraPos, Vector3 cameraForward, Vector3 cameraUp, float dt) {
	listenerLastPos = listenerPos;
	listenerPos = toFMODVector(cameraPos);
	listenerForward = toFMODVector(cameraForward);
	listenerUp = toFMODVector(cameraUp);

	listenerVelocity.x = (listenerPos.x - listenerLastPos.x) * (dt / 1000);
	listenerVelocity.y = (listenerPos.y - listenerLastPos.y) * (dt / 1000);
	listenerVelocity.z = (listenerPos.z - listenerLastPos.z) * (dt / 1000);

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

//mutes all sounds, continue playing
void AudioSystem::MuteAllSounds() {
	masterGroup->setMute(1);
}

//unmutes all sounds
void AudioSystem::UnmuteAllSounds() {
	masterGroup->setMute(0);
}

//converts vec3 to FMODVec
FMOD_VECTOR toFMODVector(Vector3 v) {
	FMOD_VECTOR fm;
	fm.x = v.x;
	fm.y = v.y;
	fm.z = v.z;
	return fm;
}
