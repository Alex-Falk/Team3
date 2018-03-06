#include "WeaponPickup.h"
#include "SimpleGamePlay.h"
#include "PaintPool.h"
#include "Game.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnInitializeScene()
{
	dimensions = Vector2(35, 50);

	Map::SetMapDimensions(dimensions);
	Map::OnInitializeScene();
}


void SimpleGamePlay::SetSpawnLocations()
{
	GUIsystem::Instance()->SetDrawScoreBar(true);
	spawnPositions[0] = Vector3(35, 5, 35);
	spawnPositions[1] = Vector3(35, 5, -35);
	spawnPositions[2] = Vector3(-35, 5, 35);
	spawnPositions[3] = Vector3(-35, 5, -35);
}

void SimpleGamePlay::AddObjects()
{

	BuildGround(dimensions);

	pickup[0] = new PaintPool(Vector3(0, 0.6f, 0), RED,"0");
	this->AddGameObject(pickup[0]);

	pickup[1] = new WeaponPickup(Vector3(20, 1.5, 20), PAINT_SPRAY, "1", 5.0f);

	this->AddGameObject(pickup[1]);
	
	mc = new MinionCamp(START_COLOUR, "0", { 0,1.5f,15 }, { 0.5f,0.5f,0.5f }, 10);
	mc2 = new MinionCamp(START_COLOUR, "1", { 0,1.5f,-30 }, { 0.5f,0.5f,0.5f }, 10);
	this->AddGameObject(mc);
	this->AddGameObject(mc2);
	AddCaptureArea(mc);
	AddCaptureArea(mc2);

	pickup[2] = new Pickup(Vector3(-10, 1.5, 7), PickupType::SPEED_BOOST, "2");

	this->AddGameObject(pickup[2]);

	pickup[3] = new Pickup(Vector3(2, 1.5, -7), PickupType::JUMP_BOOST, "3");

	this->AddGameObject(pickup[3]);

	pickup[4] = new PaintPool(Vector3(-15.0f, 0.6f, -15.0f), GREEN, "4");

	this->AddGameObject(pickup[4]);

	//add capture area
	capture[0] = new CaptureArea(Vector3(-25, 0.6, 25), "0", Vector3(3.0f, 0.5f, 3.0f), 10);
	capture[0]->SetScoreValue(100000);
	this->AddGameObject(capture[0]);
	AddCaptureArea(capture[0]);
}

//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnUpdateScene(float dt)
{
	perfMapObjects.UpdateRealElapsedTime(dt);
	Map::OnUpdateScene(dt);
	perfMapObjects.BeginTimingSection();
	for (uint i = 0; i < npickup; ++i)
	{
		if (pickup[i])
		{
			pickup[i]->Update(dt);
		}
	}
	if (mc) {
		mc->Update(dt);	
	}
	if (mc2) {
		mc2->Update(dt);
	}
	perfMapObjects.EndTimingSection();
}

//--------------------------------------------------------------------------------------------//
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}