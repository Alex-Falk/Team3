#include "WeaponPickup.h"
#include "SimpleGamePlay.h"
#include "PaintPool.h"
#include "Game.h"
#include "MultiPaintPool.h"
#include "CaptureArea.h"
#include "ParticleEmitter.h"
#include "AudioSystem.h"
#include "LaunchPad.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnInitializeScene()
{
	dimensions = Vector2(100,100);
	AudioSystem::Instance()->StopAllSounds();
	AudioSystem::Instance()->Update();
	AudioSystem::Instance()->PlayASound(GAME_MUSIC, true);

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

	BuildInvisibleWalls(dimensions);

	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Centre Position
		Vector3(dimensions.x, 1.0f, dimensions.y),		// Scale
		true,
		0.0f,
		true,
		false,								// Dragable By User
		BIG_NODE,
		Vector4(0.6f, 0.6f, 0.6f, 1.0f),
		MATERIALTYPE::Ground);	// Colour
	this->AddGameObject(ground);

	// PICKUPS 
	AddGameObject(new PaintPool(Vector3(0, 0.6f, 0), RED,"0"));
	AddGameObject(new WeaponPickup(Vector3(20, 1.5, 20), PAINT_SPRAY, "1", 5.0f));
	AddGameObject(new Pickup(Vector3(-10, 1.5, 7), PickupType::SPEED_BOOST, "2"));
	AddGameObject(new Pickup(Vector3(2, 1.5, -7), PickupType::JUMP_BOOST, "3"));
	AddGameObject(new PaintPool(Vector3(-15.0f, 0.6f, -15.0f), GREEN, "4"));

	PaintPool* cpp1 = new PaintPool(Vector3(10.0f, 0.7f, -10.0f), START_COLOUR, "5");
	PaintPool* cpp2 = new PaintPool(Vector3(10.0f, 0.7f, -20.0f), START_COLOUR, "6");
	PaintPool* cpp3 = new PaintPool(Vector3(20.0f, 0.7f, -10.0f), START_COLOUR, "7");
	PaintPool* cpp4 = new PaintPool(Vector3(20.0f, 0.7f, -20.0f), START_COLOUR, "8");
	
	AddGameObject(cpp1);
	AddGameObject(cpp2);
	AddGameObject(cpp3);
	AddGameObject(cpp4);

	// CAPTUREAREAS
	AddGameObject(new MinionCaptureArea(START_COLOUR, "0", { 0,1.5f,15 }, { 0.5f,0.5f,0.5f }, 10));
	AddGameObject(new MinionCaptureArea(START_COLOUR, "1", { 0,1.5f,-30 }, { 0.5f,0.5f,0.5f }, 10));
	
	AddGameObject(new LaunchPad(Vector3(0.0f, 1.0f, 25.0f), Vector3(2.0f, 0.01f, 2.0f), "poo"));

	MultiPaintPool* mpp = new MultiPaintPool(Vector3(15.0f, 0.6f, -15.0f), "2", Vector3(3.0f, 0.5f, 3.0f), 10);
	AddGameObject(mpp);
	mpp->AddPool(cpp1);
	mpp->AddPool(cpp2);
	mpp->AddPool(cpp3);
	mpp->AddPool(cpp4);



	//this->AddGameObject(e);
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
