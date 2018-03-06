//Michael Davis 16/02/2018
#include "MinionCaptureArea.h"

MinionCaptureArea::MinionCaptureArea() : CaptureArea() {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	maxMinions = 2;
	colour = START_COLOUR;
}

MinionCaptureArea::MinionCaptureArea(Colour col, string unique_name, Vector3 pos, Vector3 halfdims, int scoreValue) : CaptureArea(pos,unique_name,halfdims,scoreValue, col) {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	maxMinions = 2;
	colour = col;
}

MinionCaptureArea::~MinionCaptureArea() {
	for (vector<Minion*>::iterator itr = minions.begin(); itr != minions.end();) {
			itr = minions.erase(itr);			
	}
}

void MinionCaptureArea::Update(float dt) {
	currentSpawnTimer += dt;

	//if the spawntimer is over 5 seconds and there is less than 5 active minions from this spawner, spawn minion
	if (colour != START_COLOUR && currentSpawnTimer > spawnTimer && minions.size() < maxMinions) {
		Minion * m = new Minion(colour, Render()->GetchildBaseColor(), Physics()->GetPosition() + Vector3{ 0,Render()->GetBoundingRadius() * 1.2f,0 }, "Minion");
		minions.push_back(m);
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(m);
		currentSpawnTimer = 0.0f;
	}

	//iterate through vector deleting dead minions and updating alive ones
	for (vector<Minion*>::iterator itr = minions.begin(); itr != minions.end(); ) {
		if (!((*itr)->IsAlive())) {		
			itr = minions.erase(itr);
		}
		else {
			(*itr)->Update(dt);
			++itr;
		}
	}
}
