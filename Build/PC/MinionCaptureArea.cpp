//Michael Davis 16/02/2018
#include "Game.h"
#include "ParticleEmitter.h"
#include "MinionCaptureArea.h"

MinionCaptureArea::MinionCaptureArea() : CaptureArea() {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	maxMinions = 2;
	colour = START_COLOUR;
	type = MINION_CAPTURE_AREA;
}

MinionCaptureArea::MinionCaptureArea(Colour col, string unique_name, Vector3 pos, Vector3 halfdims, int scoreValue) : CaptureArea(pos,0,unique_name,halfdims,scoreValue, col) {
	spawnTimer = 5.0f;
	currentSpawnTimer = 0.0f;
	maxMinions = 10;
	colour = col;
	type = MINION_CAPTURE_AREA;

	//-Alex Falk----------------------------------------------------------//
	// Visual Effect of the Minion spawner - just a particle emitter
	e = new ParticleEmitter(
		128,
		colour,
		pos,
		(1 / 60.0f),
		0,
		{ 0.1f, 0.1f, 0.1f },
		{ 0.0f, 2.0f, 0.0f },
		360.0f,
		15.0f,
		5.0f,
		8.0f);

	renderNode->GetChild()->SetTransform(Matrix4::Scale(Vector3(0.1f,0.1f,0.1f)));
	renderNode->SetTransform(Matrix4::Translation(Vector3(pos.x, 0, pos.z)));
	//--------------------------------------------------------------------//
}

MinionCaptureArea::~MinionCaptureArea() {
}

void MinionCaptureArea::Update(float dt) {
	Map * m = static_cast<Map*>(Game::Instance()->GetMap());

	e->Update(dt);
	if (Game::Instance()->GetUserID() == 0)
	{
		if (colour != START_COLOUR)
			currentSpawnTimer += dt;

		//if the spawntimer is over 5 seconds
		if (colour != START_COLOUR && currentSpawnTimer > spawnTimer) {
			Minion * m = new Minion(colour, Render()->GetchildBaseColor(), Physics()->GetPosition() + Vector3{ 0,Render()->GetBoundingRadius() * 1.2f,0 });
			Game::Instance()->SpawnMinion(m);
			currentSpawnTimer = 0.0f;
		}
	}
}

void MinionCaptureArea::SetColour(Colour c)
{
	colour = c;

	Vector4 paintColour;

	switch (colour)
	{
	case RED:			paintColour = RED_COLOUR;		playerScores[0] = lifeReq;	playerScores[1] = 0;		playerScores[2] = 0;		playerScores[3] = 0;		break;
	case GREEN:			paintColour = GREEN_COLOUR;		playerScores[0] = 0;		playerScores[1] = lifeReq;	playerScores[2] = 0;		playerScores[3] = 0;		break;
	case BLUE:			paintColour = BLUE_COLOUR;		playerScores[0] = 0;		playerScores[1] = 0;		playerScores[2] = lifeReq;	playerScores[3] = 0;		break;
	case PINK:			paintColour = PINK_COLOUR;		playerScores[0] = 0;		playerScores[1] = 0;		playerScores[2] = 0;		playerScores[3] = lifeReq;	break;
	case START_COLOUR:	paintColour = DEFAULT_COLOUR;	playerScores[0] = 0;		playerScores[1] = 0;		playerScores[2] = 0;		playerScores[3] = 0;		break;
	}

	e->SetColour(c);
	Render()->SetChildBaseColor(paintColour);
}