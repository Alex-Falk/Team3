#pragma once

#include "Gameplay.h"
#include <ncltech/GameObject.h>

class Particle : public GameObject {
protected:
	Colour colour;
	float lifeTime = 5.0f;
public:
	Particle(Colour col, Vector3 pos, Vector3 vel, float size, string name = "");

	virtual void Update(float dt);
};