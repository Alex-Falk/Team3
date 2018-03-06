#include "WeaponPickup.h"
#include "Stage4.h"
#include "PaintPool.h"
#include "Game.h"
#include "MultiPaintPool.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void StageFourth::OnInitializeScene()
{
	dimensions = Vector2(150, 150);

	Map::SetMapDimensions(dimensions);
	Map::OnInitializeScene();
}


void StageFourth::SetSpawnLocations()
{
	GUIsystem::Instance()->SetDrawScoreBar(true);
	spawnPositions[0] = Vector3(35, 5, 35);
	spawnPositions[1] = Vector3(35, 5, -35);
	spawnPositions[2] = Vector3(-35, 5, 35);
	spawnPositions[3] = Vector3(-35, 5, -35);
}

void StageFourth::AddObjects()
{

	BuildGround(dimensions);

	pickup[0] = new PaintPool(Vector3(75, 1.f, 75), RED, "0");
	this->AddGameObject(pickup[0]);
	pickup[1] = new PaintPool(Vector3(-75, 1.f, -75), GREEN, "1");
	this->AddGameObject(pickup[1]);
	pickup[2] = new PaintPool(Vector3(75, 1.f, -75), BLUE, "2");
	this->AddGameObject(pickup[2]);
	pickup[3] = new PaintPool(Vector3(-75, 1.f, 75), PINK, "3");
	this->AddGameObject(pickup[3]);


	pickup[4] = new WeaponPickup(Vector3(0, 2.0, 70), PAINT_SPRAY, "4", 5.0f);
	this->AddGameObject(pickup[4]);
	pickup[5] = new WeaponPickup(Vector3(0, 2.0, -70), PAINT_SPRAY, "5", 5.0f);
	this->AddGameObject(pickup[5]);


	//mc = new MinionCamp(START_COLOUR, "0", { 0,0.5f,15 }, { 0.5f,0.5f,0.5f }, 10);
	//mc2 = new MinionCamp(START_COLOUR, "1", { 0,0.5f,-30 }, { 0.5f,0.5f,0.5f }, 10);
	//this->AddGameObject(mc);
	//this->AddGameObject(mc2);
	//AddCaptureArea(mc);
	//AddCaptureArea(mc2);

	pickup[6] = new Pickup(Vector3(40, 1.5, 0), PickupType::SPEED_BOOST, "6");
	this->AddGameObject(pickup[6]);

	pickup[7] = new Pickup(Vector3(-40, 1.5, 0), PickupType::JUMP_BOOST, "7");
	this->AddGameObject(pickup[7]);


	pickup[8] = new Pickup(Vector3(0, 1.5, 80), PickupType::SPEED_BOOST, "8");
	this->AddGameObject(pickup[8]);

	pickup[9] = new PaintPool(Vector3(0.f, 1.f, 0.0f), START_COLOUR, "9");
	this->AddGameObject(pickup[9]);

	//add capture area
	capture[0] = new MultiPaintPool(Vector3(0, 5, 0), "0", Vector3(2.0f, 2.f, 2.0f), 0);
	this->AddGameObject(capture[0]);
	capture[1] = new CaptureArea(Vector3(50, 2.5f, 50), "1", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[1]);
	capture[2] = new CaptureArea(Vector3(-50, 2.5f, -50), "2", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[2]);
	capture[3] = new CaptureArea(Vector3(50, 7.5f, 50), "3", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[3]);
	capture[4] = new CaptureArea(Vector3(-50, 7.5f, -50), "4", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[4]);
	capture[5] = new CaptureArea(Vector3(70, 4.f, -38), "5", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[5]);
	capture[6] = new CaptureArea(Vector3(45, 4.f, -10), "6", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[6]);
	capture[7] = new CaptureArea(Vector3(-65, 4.f, -15), "7", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[7]);
	capture[8] = new CaptureArea(Vector3(-47, 4.f, 25), "8", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[8]);
	capture[9] = new CaptureArea(Vector3(0, 4.f, 28), "9", Vector3(2.5, 2.5f, 2.5f), 1000);
	this->AddGameObject(capture[9]);

	//Add paint pool to capture area
	static_cast<MultiPaintPool*>(capture[0])->AddPool(static_cast<PaintPool*>(pickup[9]));

	//add multi capture pool



}

//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void StageFourth::OnUpdateScene(float dt)
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
	//if (mc) {
	//	mc->Update(dt);
	//}
}

//--------------------------------------------------------------------------------------------//
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void StageFourth::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}