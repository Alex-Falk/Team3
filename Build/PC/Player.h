#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\CommonUtils.h>
#include "GamePlay.h"

class Player :GameObject
{
public:
	Player();
	Player(Vector3 pos, Colour c, float s);
	~Player();
	virtual Colour GetColour() { return colour; }
	virtual void SetColour(Colour c) { colour = c; }
	virtual void SetSize(float s) { size = s; }
	virtual	float GetSize() { return size; }
private:
	int Life;
	Colour colour;
	float size =1;
};

