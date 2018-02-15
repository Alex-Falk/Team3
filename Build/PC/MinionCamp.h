#pragma once

#include "../ncltech/GameObject.h"
#include "Minion.h"
#include "../ncltech/SceneManager.h"

class MinionCamp {
protected:
	float spawnTimer;
	std::vector<Minion*> minions;

	Colour colour;
	Vector4 RGBA;

public:
	MinionCamp();
	~MinionCamp();

	void Update(float dt);
};
