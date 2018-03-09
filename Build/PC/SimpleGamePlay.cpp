#include "WeaponPickup.h"
#include "SimpleGamePlay.h"
#include "PaintPool.h"
#include "Game.h"
#include "MultiPaintPool.h"
#include "CaptureArea.h"
#include "ParticleEmitter.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnInitializeScene()
{
	dimensions = Vector2(100,100);

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

	PaintPool* cpp1 = new PaintPool(Vector3(10.0f, 0.7f, -10.0f), START_COLOUR, "5");
	PaintPool* cpp2 = new PaintPool(Vector3(10.0f, 0.7f, -20.0f), START_COLOUR, "6");
	PaintPool* cpp3 = new PaintPool(Vector3(20.0f, 0.7f, -10.0f), START_COLOUR, "7");
	PaintPool* cpp4 = new PaintPool(Vector3(20.0f, 0.7f, -20.0f), START_COLOUR, "8");
	
	AddPickup(cpp1);
	AddPickup(cpp2);
	AddPickup(cpp3);
	AddPickup(cpp4);

	// CAPTUREAREAS
	AddCaptureArea(new CaptureArea({ 5.0f,1.5f,15 } , "0", { 0.5f,0.5f,0.5f }, 10));
	AddCaptureArea(new MinionCaptureArea(START_COLOUR, "1", { 0,1.5f,15 }, { 0.5f,0.5f,0.5f }, 10));
	AddCaptureArea(new MinionCaptureArea(START_COLOUR, "2", { 0,1.5f,-30 }, { 0.5f,0.5f,0.5f }, 10));
	
	MultiPaintPool* mpp = new MultiPaintPool(Vector3(15, 0.6, -15), "3", Vector3(3.0f, 0.5f, 3.0f), 10);
	AddCaptureArea(mpp);
	mpp->AddPool(cpp1);
	mpp->AddPool(cpp2);
	mpp->AddPool(cpp3);
	mpp->AddPool(cpp4);

	ParticleEmitter * e = new ParticleEmitter(RED, { 0,2,0 }, 20, { 1,1,0 }, 10,10,5,50);
	this->AddGameObject(e);
}

//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnUpdateScene(float dt)
{
	perfMapObjects.UpdateRealElapsedTime(dt);
	Map::OnUpdateScene(dt);
	
}

//--------------------------------------------------------------------------------------------//
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}
