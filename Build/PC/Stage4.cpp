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

	AddPickup(new PaintPool(Vector3(135, 1.f, 135), RED, "0"));
	AddPickup(new PaintPool(Vector3(-135, 1.f, -135), GREEN, "1"));
	AddPickup(new PaintPool(Vector3(135, 1.f, -135), BLUE, "2"));
	AddPickup(new PaintPool(Vector3(-135, 1.f, 135), PINK, "3"));


	AddPickup(new WeaponPickup(Vector3(0, 2.0, 70), PAINT_SPRAY, "4", 5.0f));
	AddPickup(new WeaponPickup(Vector3(0, 2.0, -70), PAINT_SPRAY, "5", 5.0f));


	//mc = new MinionCamp(START_COLOUR, "0", { 0,0.5f,15 }, { 0.5f,0.5f,0.5f }, 10);
	//mc2 = new MinionCamp(START_COLOUR, "1", { 0,0.5f,-30 }, { 0.5f,0.5f,0.5f }, 10);
	//this->AddGameObject(mc);
	//this->AddGameObject(mc2);
	//AddCaptureArea(mc);
	//AddCaptureArea(mc2);

	AddPickup(new Pickup(Vector3(40, 1.5, 0), PickupType::SPEED_BOOST, "6"));

	AddPickup(new Pickup(Vector3(-40, 1.5, 0), PickupType::JUMP_BOOST, "7"));


	AddPickup(new Pickup(Vector3(0, 1.5, 80), PickupType::SPEED_BOOST, "8"));

	AddPickup(new PaintPool(Vector3(0.f, 1.f, 0.0f), START_COLOUR, "9"));

	//add capture area
	AddCaptureArea(new MultiPaintPool(Vector3(0, 5, 0), "0", Vector3(2.0f, 2.f, 2.0f), 0));
	AddCaptureArea(new CaptureArea(Vector3(50, 2.5f, 50), "1", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(-50, 2.5f, -50), "2", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(50, 7.5f, 50), "3", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(-50, 7.5f, -50), "4", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(70, 4.f, -38), "5", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(45, 4.f, -10), "6", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(-65, 4.f, -15), "7", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(-47, 4.f, 25), "8", Vector3(2.5, 2.5f, 2.5f), 1000));
	AddCaptureArea(new CaptureArea(Vector3(0, 4.f, 28), "9", Vector3(2.5, 2.5f, 2.5f), 1000));

	//Add paint pool to capture area
	static_cast<MultiPaintPool*>(GetCaptureArea(0))->AddPool(static_cast<PaintPool*>(GetPickup(9)));

}

//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void StageFourth::OnUpdateScene(float dt)
{
	perfMapObjects.UpdateRealElapsedTime(dt);
	Map::OnUpdateScene(dt);
	perfMapObjects.BeginTimingSection();
	for (uint i = 0; i < pickups.size(); ++i)
	{
		if (GetPickups()[i])
		{
			GetPickups()[i]->Update(dt);
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