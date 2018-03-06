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

	// PICKUPS 
	AddPickup(new PaintPool(Vector3(0, 0.6f, 0), RED,"0"));
	AddPickup(new WeaponPickup(Vector3(20, 1.5, 20), PAINT_SPRAY, "1", 5.0f));
	AddPickup(new Pickup(Vector3(-10, 1.5, 7), PickupType::SPEED_BOOST, "2"));
	AddPickup(new Pickup(Vector3(2, 1.5, -7), PickupType::JUMP_BOOST, "3"));
	AddPickup(new PaintPool(Vector3(-15.0f, 0.6f, -15.0f), GREEN, "4"));

	for (auto itr = pickups.begin(); itr != pickups.end(); ++itr)
	{
		this->AddGameObject((*itr));
	}

	// CAPTUREAREAS
	AddCaptureArea(new MinionCaptureArea(START_COLOUR, "0", { 0,1.5f,15 }, { 0.5f,0.5f,0.5f }, 10));
	AddCaptureArea(new MinionCaptureArea(START_COLOUR, "1", { 0,1.5f,-30 }, { 0.5f,0.5f,0.5f }, 10));
	AddCaptureArea(new CaptureArea(Vector3(15, 0.6, -15), "2", Vector3(3.0f, 0.5f, 3.0f), 10));

	for (auto itr = captureAreas.begin(); itr != captureAreas.end(); ++itr)
	{
		this->AddGameObject((*itr));
	}

}

//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnUpdateScene(float dt)
{
	perfMapObjects.UpdateRealElapsedTime(dt);
	Map::OnUpdateScene(dt);
	perfMapObjects.BeginTimingSection();

	for (auto itr = pickups.begin(); itr != pickups.end(); ++itr)
	{
		(*itr)->Update(dt);
	}

	for (auto itr = captureAreas.begin(); itr != captureAreas.end(); ++itr)
	{
		if (Game::Instance()->getUserID() == 0)
			(*itr)->Update(dt);
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