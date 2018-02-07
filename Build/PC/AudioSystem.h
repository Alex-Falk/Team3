#pragma once
#include <fmod.hpp>
#include <iostream>

//Michael Davis - 07/02/2018

class AudioSystem {
public:
	AudioSystem() {
		FMOD::System_Create(&audioSystem);
		int driverCount = 0;
		audioSystem->getNumDrivers(&driverCount);
		if (driverCount == 0) {
			std::cout << "No Audio Driver Detected!" << std::endl;
		}
	}
private:
	FMOD::System * audioSystem;

};