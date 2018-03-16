////////////////////////////////////////////////
//         Philip Beck 07/02/2018             //
////////////////////////////////////////////////

#pragma once
#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include <nclgl\TSingleton.h>

enum InputType{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP,
	SHOOT,
	PAUSE,
	CAMERA_UP,
	CAMERA_DOWN,
	DEBUGKEY,
	//must stay at the bottom, this is the size of the array
	COUNT
};


//TODO: THIS SHOULD BE A SINGLETON BUT IT BREAKS THE CODE SO A GLOBAL VARIABLE
//IS BEING USED INSTEAD
class Input: public TSingleton<Input> {
	friend class TSingleton <Input>;
public:
	inline bool GetInput(InputType i) {
		return inputs[i]; 
	}
	
	inline void SetInput(InputType i, bool b) {
		inputs[i] = b; 
	};

	bool ToggleInput(InputType i) {
		inputs[i] = !inputs[i];
		return inputs[i];
	}

	inline float GetLookX() { return lookX; }
	inline void SetLookX(float x) { lookX = x; }

	inline float GetLookY() { return lookY; }
	inline void SetLookY(float y) { lookY = y; }
private:
	Input() {
		for (int i = 0; i < InputType::COUNT; i++) {
			inputs[i] = false;
		}
	}
	bool inputs[InputType::COUNT];
	float lookX;
	float lookY;
};

#endif