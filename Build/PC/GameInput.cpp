#include "GameInput.h"


//TODO REPLACE WITH SINGLETON
Input globalInput;

Input* Input::GetInput() {
	return &globalInput;
}