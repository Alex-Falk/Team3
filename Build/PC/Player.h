#pragma once
#include "GamePlay.h"

class Player
{
public:
	Player();
	~Player();
	int GetColour();

private:
	int Life;
	Colour colour;
};

Player::Player()
{
}

Player::~Player()
{
}