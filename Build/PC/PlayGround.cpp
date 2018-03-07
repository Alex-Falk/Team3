// Daniel Burns [12/02/2018] Testing Map
#include "PlayGround.h"
#include "PaintPool.h"
#include "Game.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//

void PlayGround::OnInitializeScene()
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
	//GameplayTesting();		// Testing Functionality.

	pickupPool = new PaintPool(Vector3(10, 1.0f, 10), RED); this->AddGameObject(pickupPool);
	pickupPool = new PaintPool(Vector3(-13, 1.0f, 0), START_COLOUR); this->AddGameObject(pickupPool);

	// Score & GUI initialisation -------------------------------------------------------------------------------------------------
	OnInitializeGUI();

	// General Initialization -----------------------------------------------------------------------------------------------------
	xDimension = 140;
	yDimension = 80;
	groundScoreAccuracy = 15;
	Window::GetWindow().LockMouseToWindow(true);
	Map::OnInitializeScene();
}

void PlayGround::SetSpawnLocations()
{
	spawnPositions[0] = Vector3(25, 5, 25);
	spawnPositions[1] = Vector3(25, 5, -25);
	spawnPositions[2] = Vector3(-25, 5, 25);
	spawnPositions[3] = Vector3(-25, 5, -25);
}

void PlayGround::OnUpdateScene(float dt)
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

	if (pickupWeapon)
	{
		pickupWeapon->Update(dt);
	}


	for (uint i = 0; i < npickup; ++i)
	{
		if (pickup[i])
		{
			pickup[i]->Update(dt);
		}
	}
}

void PlayGround::CreateEnvironment()
{
	// Level Floor's
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Position = Right+---Up+---Foward+
		Vector3(140.0f, 1.0f, 80.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(ground);

	GameObject* Level2Left = CommonUtils::BuildCuboidObject(
		"Level2Left",
		Vector3(-192.17f, 13.345f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(25.0f, 0.3f, 80.0f),	// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(Level2Left);

	GameObject* Level2Right = CommonUtils::BuildCuboidObject(
		"Level2Right",
		Vector3(192.17f, 13.345f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(25.0f, 0.3f, 80.0f),	// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(Level2Right);

	// Ramps
	GameObject* Ramp = CommonUtils::BuildCuboidObject(
		"Ramp",
		Vector3(-154.0f, 6.4f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(15.0f, 1.0f, 80.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(25, Vector3(0, 0, 1))));
	this->AddGameObject(Ramp);

	GameObject* Ramp2 = CommonUtils::BuildCuboidObject(
		"Ramp2",
		Vector3(154.0f, 6.4f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(15.0f, 1.0f, 80.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(0, 0, 1))));
	this->AddGameObject(Ramp2);

	// Walls
	GameObject* Wall = CommonUtils::BuildCuboidObject(
		"Wall",
		Vector3(112.5f, 21.0f, 82.0f),	// Position = Right+---Up+---Foward+
		Vector3(105.5f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Wall);

	GameObject* Wall2 = CommonUtils::BuildCuboidObject(
		"Wall2",
		Vector3(-112.5f, 21.0f, 82.0f),	// Position = Right+---Up+---Foward+
		Vector3(105.5f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Wall2);

	GameObject* Wall3 = CommonUtils::BuildCuboidObject(
		"Wall3",
		Vector3(112.5f, 21.0f, -82.0f),	// Position = Right+---Up+---Foward+
		Vector3(105.5f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Wall3);

	GameObject* Wall4 = CommonUtils::BuildCuboidObject(
		"Wall4",
		Vector3(-112.5f, 21.0f, -82.0f),	// Position = Right+---Up+---Foward+
		Vector3(105.5f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Wall4);
	
	// Objects
	GameObject* BouncePad = CommonUtils::BuildCuboidObject(
		"BouncePad",
		Vector3(0.0f, 0.1f, 84.0f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 7.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BouncePad);

	GameObject* BouncePad2 = CommonUtils::BuildCuboidObject(
		"BouncePad2",
		Vector3(0.0f, 0.1f, -84.0f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 7.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BouncePad2);

	GameObject* BouncePadWall = CommonUtils::BuildCuboidObject(
		"BouncePadWall",
		Vector3(0.0f, 21.1f, 93.0f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 20.0f, 2.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BouncePadWall);

	GameObject* BouncePadWall2 = CommonUtils::BuildCuboidObject(
		"BouncePadWall2",
		Vector3(0.0f, 21.1f, -93.0f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 20.0f, 2.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BouncePadWall2);

	/*	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Position = Right+---Up+---Foward+
		Vector3(150.0f, 1.0f, 150.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(ground);

	GameObject* Box = CommonUtils::BuildCuboidObject(
		"Box",
		Vector3(0.0f, 4.0f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(8.0f, 3.0f, 8.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Box);

	GameObject* Ramp = CommonUtils::BuildCuboidObject(
		"Ramp",
		Vector3(0.0f, 3.0f, 30.0f),	// Position = Right+---Up+---Foward+
		Vector3(8.0f, 1.0f, 8.6f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(1, 0, 0))));
	this->AddGameObject(Ramp);

	GameObject* Ramp2 = CommonUtils::BuildCuboidObject(
		"Ramp2",
		Vector3(0.0f, 3.0f, -30.0f),	// Position = Right+---Up+---Foward+
		Vector3(8.0f, 1.0f, 8.6f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(1, 0, 0))));
	this->AddGameObject(Ramp2);

	GameObject* Ramp3 = CommonUtils::BuildCuboidObject(
		"Ramp3",
		Vector3(30.0f, 3.0f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(8.6f, 1.0f, 8.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp3->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(0, 0, 1))));
	this->AddGameObject(Ramp3);

	GameObject* Ramp4 = CommonUtils::BuildCuboidObject(
		"Ramp4",
		Vector3(-30.0f, 3.0f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(8.6f, 1.0f, 8.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(0, 0, 1))));
	this->AddGameObject(Ramp4);*/

	//pickupPool = new PaintPool(Vector3(-13, 1.0f, 5), START_COLOUR); this->AddGameObject(pickupPool);
}
