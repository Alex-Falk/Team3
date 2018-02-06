#pragma once

#include "Player.h"
#include "GamePlay.h"

class Game 
{
public:
	static Game* GetInstance()
	{
		if (!game) {
			game = new Game();
		}
		return game;
	}
private:
	static Game* game;
	//private constructor
	Game();
};