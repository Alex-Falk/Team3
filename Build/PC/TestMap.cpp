// Daniel Burns [12/02/2018] Testing Map
#include "TestMap.h"
#include "PaintPool.h"
#include "Game.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void TestMap::OnInitializeScene()
{
	// Loading Textures -----------------------------------------------------------------------------------------------------------
	GraphicsPipeline::Instance()->SetIsMainMenu(false);
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	if (!TextureManager::Instance()->LoadCubeMap(TEXTURETYPE::Sky_Box, TEXTUREDIR"SkyBox\\skyright.jpg", TEXTUREDIR"SkyBox\\skyleft.jpg", TEXTUREDIR"SkyBox\\skytop.jpg",
		TEXTUREDIR"SkyBox\\skybottom.jpg", TEXTUREDIR"SkyBox\\skyback.jpg", TEXTUREDIR"SkyBox\\skyfront.jpg"))
		NCLERROR("Texture not loaded");

	// Loading Environment Objects ------------------------------------------------------------------------------------------------
	CreateEnvironment();	// Generates Environment.
	SetSpawnLocations();
	GameplayTesting();		// Testing Functionality.						

	// Loading Pickups ------------------------------------------------------------------------------------------------------------
	pickupSpeedBoost = new Pickup(Vector3(-12, 3, 5), SPEED_BOOST); 
	this->AddGameObject(pickupSpeedBoost);

	pickupJumpBoost = new Pickup(Vector3(-12, 3, -5), JUMP_BOOST); 
	this->AddGameObject(pickupJumpBoost);

	pickupPaintSpray = new WeaponPickup(Vector3(12, 3, -8), PAINT_SPRAY);
	this->AddGameObject(pickupPaintSpray);

	pickupPaintPistol = new WeaponPickup(Vector3(12, 3, 8), PAINT_PISTOL);
	this->AddGameObject(pickupPaintPistol);

	pickupAutoPaintLauncher = new WeaponPickup(Vector3(16, 3, 4), AUTO_PAINT_LAUNCHER);
	this->AddGameObject(pickupAutoPaintLauncher);

	pickupPaintRocket = new WeaponPickup(Vector3(16, 3, -4), PAINT_ROCKET);
	this->AddGameObject(pickupPaintRocket);

	// Loading PaintPools ---------------------------------------------------------------------------------------------------------
	pickupPool = new PaintPool(Vector3(0, 1.1f, 6), RED); this->AddGameObject(pickupPool);
	pickupPool = new PaintPool(Vector3(0, 1.1f, -6), GREEN); this->AddGameObject(pickupPool);
	pickupPool = new PaintPool(Vector3(0, 2.5f, 20), BLUE); this->AddGameObject(pickupPool);
	pickupPool = new PaintPool(Vector3(0, 2.5f, -20), PINK); this->AddGameObject(pickupPool);

	// Score & GUI initialisation -------------------------------------------------------------------------------------------------
	OnInitializeGUI();

	// General Initialization -----------------------------------------------------------------------------------------------------
	xDimension = 40;
	yDimension = 40;
	groundScoreAccuracy = 15;

	Map::OnInitializeScene();
}

void TestMap::SetSpawnLocations()
{
	spawnPositions[0] = Vector3(25, 5, 25);
	spawnPositions[1] = Vector3(25, 5, -25);
	spawnPositions[2] = Vector3(-25, 5, 25);
	spawnPositions[3] = Vector3(-25, 5, -25);
}



//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void TestMap::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	for (uint i = 0; i < 4; i++) {
		if (Game::Instance()->GetPlayer(i))
			Game::Instance()->GetPlayer(i)->OnAvatarUpdate(dt);
	}

	if (pickupPool)
	{
		pickupPool->Update(dt);
	}

	if (pickupJumpBoost)
	{
		pickupJumpBoost->Update(dt);
	}

	if (pickupSpeedBoost)
	{
		pickupSpeedBoost->Update(dt);
	}

	if (pickupPaintSpray)
	{
		pickupPaintSpray->Update(dt);
	}

	if (pickupPaintPistol)
	{
		pickupPaintPistol->Update(dt);
	}

	if (pickupAutoPaintLauncher)
	{
		pickupAutoPaintLauncher->Update(dt);
	}

	if (pickupPaintRocket)
	{
		pickupPaintRocket->Update(dt);
	}

	for (uint i = 0; i < npickup; ++i)
	{
		if (pickup[i])
		{
			pickup[i]->Update(dt);
		}
	}

	// Display Pickup Text ------------------------------------------------------------------------------------------------------
	Vector3 pickupSpeedBoostText = Vector3(0.0, 2.0, 0.0);
	Vector3 pickupJumpBoostText = Vector3(0.0, 2.0, 0.0);
	Vector3 pickupPaintSpray = Vector3(0.0, 2.0, 0.0);
	Vector3 pickupPaintPistol = Vector3(0.0, 2.0, 0.0);
	Vector3 pickupAutoPaintLauncher = Vector3(0.0, 2.0, 0.0);
	Vector3 pickupPaintRocket = Vector3(0.0, 2.0, 0.0);

	NCLDebug::DrawTextWs(pickupSpeedBoostText + Vector3(-12.0f, 2.8f, 5.0f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "SPEED");
	NCLDebug::DrawTextWs(pickupJumpBoostText + Vector3(-12.0f, 2.8f, -5.0f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "JUMP");
	NCLDebug::DrawTextWs(pickupPaintSpray+ Vector3(12.0f, 2.8f, -8.0f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "PAINT-SPRAY");
	NCLDebug::DrawTextWs(pickupPaintPistol + Vector3(12.0f, 2.8f, 8.0f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "PAINT-PISTOL");
	NCLDebug::DrawTextWs(pickupPaintRocket + Vector3(16.0f, 2.8f, -4.0f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "PAINT-ROCKET");
	NCLDebug::DrawTextWs(pickupAutoPaintLauncher + Vector3(16.0f, 2.8f, 4.0f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "AUTOMATIC-LAUNCHER");
}

void TestMap::CreateEnvironment()
{
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Centre Position
		Vector3(40.0f, 1.5f, 40.0f),		// Scale
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(ground);

	// Create FrontWall (..everybody loves finding some common ground)
	GameObject* FrontWall = CommonUtils::BuildCuboidObject(
		"FrontWall",
		Vector3(0.0f, 2.5f, -41.0f),	// Position = Right+---Up+---Foward+
		Vector3(40.0f, 1.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(FrontWall);

	// Create BackWall (..everybody loves finding some common ground)
	GameObject* BackWall = CommonUtils::BuildCuboidObject(
		"BackWall",
		Vector3(0.0f, 2.5f, 41.0f),		// Position = Right+---Up+---Foward+
		Vector3(40.0f, 1.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BackWall);

	// Create LeftWall (..everybody loves finding some common ground)
	GameObject* LeftWall = CommonUtils::BuildCuboidObject(
		"LeftWall",
		Vector3(-41.0f, 2.5f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(1.0f, 1.0f, 40.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(LeftWall);

	// Create RightWall (..everybody loves finding some common ground)
	GameObject* RightWall = CommonUtils::BuildCuboidObject(
		"RightWall",
		Vector3(41.0f, 2.5f, 0.0f),		// Position = Right+---Up+---Foward+
		Vector3(1.0f, 1.0f, 40.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(RightWall);
}

void TestMap::GameplayTesting()
{
	object = CommonUtils::BuildCuboidObject("PaintableObject1",
		Vector3(-0.0f, 2.2f, -20.0f),
		Vector3(3.5f, 0.7f, 5.0f),
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PhysNodeType::DEFAULT_PHYSICS,
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),	    //Colour	
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(object);

	object = CommonUtils::BuildCuboidObject("PaintableObject2",
		Vector3(0.0f, 2.2f, 20.0f),
		Vector3(3.5f, 0.7f, 5.0f),
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PhysNodeType::DEFAULT_PHYSICS,
		Vector4(1.0f, 1.0f, 1.0f, 0.5f),	    //Colour	
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(object);
}
