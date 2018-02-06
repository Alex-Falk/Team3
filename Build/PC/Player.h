#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\CommonUtils.h>
#include "GamePlay.h"

class Player : GameObject
{
public:
	Player();
	Player(Vector3 pos, Colour c, float s);
	~Player();
	virtual Colour GetColour() { return colour; }
	virtual void SetColour(Colour c) { colour = c; }

	virtual void SetSize(float s) { size = s; }
	virtual	float GetSize() { return size; }

	GameObject* GetObj() { return Pl; }

	void Input(float time);

private:

	GameObject* Pl;

	Vector3 force = Vector3(0, 0, 0);

	Colour colour;

	int Life;

	float size = 1;
	float jumpImpulse = 10.f;
	float speed = 5;
	float Jumptime = 0;

	bool CanJump;
};

