//Michael Davis 16/02/2018
#pragma once

#include "../ncltech/GameObject.h"
#include "../ncltech/SceneManager.h"
#include "Minion.h"
#include "CaptureArea.h"

class MinionCaptureArea : public CaptureArea {
protected:
	float spawnTimer; //time that a minion will spawn at
	float currentSpawnTimer; //current time since last spawn
	std::vector<MinionBase*> minions; 
	int maxMinions; //amount of minions that can be spawned in at once by this spawner

public:
	MinionCaptureArea();
	MinionCaptureArea(Colour col, string unique_name, Vector3 pos, Vector3 halfdims, int scoreValue);
	
	void SetMaxMinions(int mm) {maxMinions = mm;}
	int GetMaxMinions() { return maxMinions; }

	void SetSpawnTimer(float f) { spawnTimer = f;}
	float GetSpawnTimer() { return spawnTimer; }

	void SpawnMinion();
	MinionBase * GetMinion(uint i) { return minions[i]; }

	virtual void Update(float dt);
	
	~MinionCaptureArea();
};
