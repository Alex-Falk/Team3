//Michael Davis 16/02/2018
#include "MinionCaptureArea.h"
#include "Game.h"

MinionCaptureArea::MinionCaptureArea() : CaptureArea() {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	maxMinions = 2;
	colour = START_COLOUR;
	type = MINION_CAPTURE_AREA;
}

MinionCaptureArea::MinionCaptureArea(Colour col, string unique_name, Vector3 pos, Vector3 halfdims, int scoreValue) : CaptureArea(pos,unique_name,halfdims,scoreValue, col) {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	maxMinions = 10;
	colour = col;
	type = MINION_CAPTURE_AREA;
}

MinionCaptureArea::~MinionCaptureArea() {
}

void MinionCaptureArea::Update(float dt) {
	Map * m = static_cast<Map*>(Game::Instance()->GetMap());

	if (Game::Instance()->getUserID() == 0)
	{
		if (colour != START_COLOUR)
			currentSpawnTimer += dt;

		//if the spawntimer is over 5 seconds and there is less than 5 active minions from this spawner, spawn minion
		if (colour != START_COLOUR && currentSpawnTimer > spawnTimer) {
			Minion * m = new Minion(colour, Render()->GetchildBaseColor(), Physics()->GetPosition() + Vector3{ 0,Render()->GetBoundingRadius() * 1.2f,0 });
			Game::Instance()->SpawnMinion(m);
			currentSpawnTimer = 0.0f;
		}
	}
}


