//Michael Davis 16/02/2018
#pragma once

#include "../ncltech/GameObject.h"
#include "Minion.h"
#include "CaptureArea.h"

class ParticleEmitter;

class MinionCaptureArea : public CaptureArea {
protected:
	float spawnTimer; //time that a minion will spawn at
	float currentSpawnTimer; //current time since last spawn
	int maxMinions; //amount of minions that can be spawned in at once by this spawner
	ParticleEmitter * e;

public:
	MinionCaptureArea();
	MinionCaptureArea(Colour col, string unique_name, Vector3 pos, Vector3 halfdims, int scoreValue = 100);

	virtual void SetColour(Colour c);

	void SetMaxMinions(int mm) {maxMinions = mm;}
	int GetMaxMinions() { return maxMinions; }

	void SetSpawnTimer(float f) { spawnTimer = f;}
	float GetSpawnTimer() { return spawnTimer; }

	virtual void Update(float dt);
	
	~MinionCaptureArea();
};
