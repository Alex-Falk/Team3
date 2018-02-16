//Michael Davis 16/02/2018
#include "MinionCamp.h"

MinionCamp::MinionCamp() {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	colour = RED;
	RGBA = RED_COLOUR;
	maxMinions = 5;
	position = { 0,0,0 };
}

MinionCamp::MinionCamp(Colour col, Vector4 rgba, Vector3 pos) {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	colour = col;
	RGBA = rgba;
	position = pos;
	maxMinions = 5;
}

MinionCamp::~MinionCamp() {
	for (vector<Minion*>::iterator itr = minions.begin(); itr != minions.end();) {
			SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(*itr);
			delete * itr;
			itr = minions.erase(itr);			
	}
}

void MinionCamp::Update(float dt) {
	currentSpawnTimer += dt;

	//if the spawntimer is over 5 seconds and there is less than 5 active minions from this spawner, spawn minion
	if (currentSpawnTimer > spawnTimer && minions.size() < maxMinions) {
		Minion * m = new Minion(colour, RGBA, position, "Minion");
		minions.push_back(m);
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(m);
		currentSpawnTimer = 0.0f;
	}

	//iterate through vector deleting dead minions and updating alive ones
	for (vector<Minion*>::iterator itr = minions.begin(); itr != minions.end(); ) {
		if ((*itr)->GetDead()) {
			SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(*itr);
			delete * itr;
			itr = minions.erase(itr);
		}
		else {
			(*itr)->Update(dt);
			++itr;
		}
	}
}
