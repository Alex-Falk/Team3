#pragma once

#include "Gameplay.h"
#include <ncltech/GameObject.h>

class Particle : public GameObject {
protected:
	Colour colour;
	Vector3 RGB;
	float lifeTime = 5.0f;
	Vector3 position;
	Vector3 velocity;
	Vector3 spawnPos;
public:
	Particle(Colour col, Vector3 pos, Vector3 vel, float size, string name = "");

	virtual void Update(float dt);
};