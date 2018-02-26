//Michael Davis 16/02/2018
#pragma once

#include "../ncltech/GameObject.h"
#include "../ncltech/SceneManager.h"
#include "Minion.h"

//TODO turn into capturable object when implemented 
class MinionCamp {
protected:
	float spawnTimer; //time that a minion will spawn at
	float currentSpawnTimer; //current time since last spawn
	std::vector<Minion*> minions; 
	int maxMinions; //amount of minions that can be spawned in at once by this spawner

	Colour colour;
	Vector4 RGBA;
	Vector3 position;

public:
	MinionCamp();
	MinionCamp(Colour col, Vector4 rgba, Vector3 pos);
	
	void SetMaxMinions(int mm) {maxMinions = mm;}
	int GetMaxMinions() { return maxMinions; }


	void SetSpawnTimer(float f) { spawnTimer = f;}
	float GetSpawnTimer() { return spawnTimer; }

	void SetPosition(Vector3 pos) { position = pos; }
	Vector3 GetPosition() { return position; }

	void Update(float dt);
	
	~MinionCamp();
};
