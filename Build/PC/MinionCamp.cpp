#include "MinionCamp.h"

MinionCamp::MinionCamp() {
	spawnTimer = 0.0f;
	colour = RED;
	RGBA = RED_COLOUR;
}

MinionCamp::~MinionCamp() {
	for (vector<Minion*>::iterator itr = minions.begin(); itr != minions.end(); ++itr) {
			SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(*itr);
			delete * itr;
			itr = minions.erase(itr);			
	}
}

void MinionCamp::Update(float dt) {
	spawnTimer += dt;

	if (spawnTimer > 5.0f && minions.size() < 1) {
		Minion * m = new Minion(colour, RGBA, { 0.0f,1.5f,0.0f }, "Minion");
		minions.push_back(m);
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(m);
		spawnTimer = 0.0f;
	}

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
