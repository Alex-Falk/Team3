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

	pickupPool = new PaintPool(Vector3(30, 1.0f, 0), RED); this->AddGameObject(pickupPool);
	
	// River
	pickupPool = new PaintPool(Vector3(0, 0.6, 0), START_COLOUR); 
	((PaintPool*)pickupPool)->ChangeSize({ 20, 0.5, 80.0 });
	this->AddGameObject(pickupPool);

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
		Vector3(-207.17f, 13.345f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(40.0f, 0.3f, 80.0f),	// Scale	= Width+---Length+---Depth+
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
		Vector3(207.17f, 13.345f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(40.0f, 0.3f, 80.0f),	// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(Level2Right);

	GameObject* Level3Front = CommonUtils::BuildCuboidObject(
		"Level3Front",
		Vector3(0.0f, 37.0f, -127.8f),			// Position = Right+---Up+---Foward+
		Vector3(128.7f, 1.0f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(Level3Front);

	GameObject* Level3Back = CommonUtils::BuildCuboidObject(
		"Level3Back",
		Vector3(0.0f, 37.0f, 127.8f),			// Position = Right+---Up+---Foward+
		Vector3(128.7f, 1.0f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(Level3Back);

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

	GameObject* JumpRamp = CommonUtils::BuildCuboidObject(
		"JumpRamp",
		Vector3(0.0f, 13.8f, -37.15f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 0.2f, 3.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp);

	GameObject* JumpRamp2 = CommonUtils::BuildCuboidObject(
		"JumpRamp",
		Vector3(0.0f, 13.8f, 37.15f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 0.2f, 3.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp2);

	
	// Stairs Front
	GameObject* Stairs = CommonUtils::BuildCuboidObject(
		"Stairs",
		Vector3(-187.2f, 18.3f, -94.2f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(1, 0, 0))));
	this->AddGameObject(Stairs);

	GameObject* Stairway = CommonUtils::BuildCuboidObject(
		"Stairway",
		Vector3(-187.2f, 23.43f, -123.15f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Stairway);

	GameObject* Stairs2 = CommonUtils::BuildCuboidObject(
		"Stairs2",
		Vector3(-147.8f, 30.5f, -125.25f),	// Position = Right+---Up+---Foward+
		Vector3(20.5f, 0.5f, 12.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(0, 0, 1))));
	this->AddGameObject(Stairs2);

	GameObject* Stairs3 = CommonUtils::BuildCuboidObject(
		"Stairs3",
		Vector3(187.2f, 18.3f, -94.2f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs3->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(1, 0, 0))));
	this->AddGameObject(Stairs3);

	GameObject* Stairway2 = CommonUtils::BuildCuboidObject(
		"Stairway2",
		Vector3(187.2f, 23.43f, -123.15f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Stairway2);

	GameObject* Stairs4 = CommonUtils::BuildCuboidObject(
		"Stairs4",
		Vector3(147.8f, 30.5f, -125.25f),	// Position = Right+---Up+---Foward+
		Vector3(20.5f, 0.5f, 12.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(0, 0, 1))));
	this->AddGameObject(Stairs4);

	// Stairs Back
	GameObject* Stairs5 = CommonUtils::BuildCuboidObject(
		"Stairs5",
		Vector3(-187.2f, 18.3f, 94.2f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs5->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(1, 0, 0))));
	this->AddGameObject(Stairs5);

	GameObject* Stairway4 = CommonUtils::BuildCuboidObject(
		"Stairway4",
		Vector3(-187.2f, 23.43f, 123.15f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Stairway4);

	GameObject* Stairs6 = CommonUtils::BuildCuboidObject(
		"Stairs6",
		Vector3(-147.8f, 30.5f, 125.25f),	// Position = Right+---Up+---Foward+
		Vector3(20.5f, 0.5f, 12.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs6->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(0, 0, 1))));
	this->AddGameObject(Stairs6);

	GameObject* Stairs7 = CommonUtils::BuildCuboidObject(
		"Stairs7",
		Vector3(187.2f, 18.3f, 94.2f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs7->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(1, 0, 0))));
	this->AddGameObject(Stairs7);

	GameObject* Stairway3 = CommonUtils::BuildCuboidObject(
		"Stairway8",
		Vector3(187.2f, 23.43f, 123.15f),	// Position = Right+---Up+---Foward+
		Vector3(20.0f, 0.5f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Stairway3);

	GameObject* Stairs8 = CommonUtils::BuildCuboidObject(
		"Stairs8",
		Vector3(147.8f, 30.5f, 125.25f),	// Position = Right+---Up+---Foward+
		Vector3(20.5f, 0.5f, 12.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Stairs8->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(0, 0, 1))));
	this->AddGameObject(Stairs8);

	// Walls
	GameObject* Wall = CommonUtils::BuildCuboidObject(
		"Wall",
		Vector3(87.1f, 21.0f, 82.0f),	// Position = Right+---Up+---Foward+
		Vector3(80.1f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
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
		Vector3(-87.1f, 21.0f, 82.0f),	// Position = Right+---Up+---Foward+
		Vector3(80.1f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
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
		Vector3(87.1f, 21.0f, -82.0f),	// Position = Right+---Up+---Foward+
		Vector3(80.1f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
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
		Vector3(-87.1f, 21.0f, -82.0f),	// Position = Right+---Up+---Foward+
		Vector3(80.1f, 20.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Wall4);

	GameObject* BackWall = CommonUtils::BuildCuboidObject(
		"BackWall",
		Vector3(-248.17f, 27.2f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(1.0f, 13.7f, 80.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BackWall);

	GameObject* BackWall2 = CommonUtils::BuildCuboidObject(
		"BackWall",
		Vector3(248.17f, 27.2f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(1.0f, 13.7f, 80.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BackWall2);
	
	// Objects
	GameObject* Bridge = CommonUtils::BuildCuboidObject(
		"Bridge",
		Vector3(0.0f, 12.0f, -60.0f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 20.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Bridge);

	GameObject* Bridge2 = CommonUtils::BuildCuboidObject(
		"Bridge2",
		Vector3(0.0f, 12.0f, 60.0f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 20.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Bridge2);

	GameObject* BouncePadWall = CommonUtils::BuildCuboidObject(
		"BouncePadWall",
		Vector3(0.0f, 33.7f, -80.3f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 7.3f, 0.3f),			// Scale	= Width+---Length+---Depth+
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
		Vector3(0.0f, 8.5f, -82.0f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 7.5f, 2.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BouncePadWall2);

	GameObject* BouncePadWall3 = CommonUtils::BuildCuboidObject(
		"BouncePadWall3",
		Vector3(0.0f, 33.7f, 80.3f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 7.3f, 0.3f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BouncePadWall3);

	GameObject* BouncePadWall4 = CommonUtils::BuildCuboidObject(
		"BouncePadWall2",
		Vector3(0.0f, 8.5f, 82.0f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 7.5f, 2.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(BouncePadWall4);

    // Jump Barriers
	GameObject* JumpWallBarrier = CommonUtils::BuildCuboidObject(
		"JumpWallBarrier",
		Vector3(0.0f, 39.0f, -108.5f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 3.0f, 1.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpWallBarrier->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(10, Vector3(1, 0, 0))));
	this->AddGameObject(JumpWallBarrier);

	GameObject* JumpWallBarrier2 = CommonUtils::BuildCuboidObject(
		"JumpWallBarrier2",
		Vector3(0.0f, 43.83f, -110.9f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 2.0f, 1.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpWallBarrier2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(50, Vector3(1, 0, 0))));
	this->AddGameObject(JumpWallBarrier2);

	GameObject* JumpWallBarrier3 = CommonUtils::BuildCuboidObject(
		"JumpWallBarrier3",
		Vector3(0.0f, 39.0f, 108.5f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 3.0f, 1.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpWallBarrier3->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-10, Vector3(1, 0, 0))));
	this->AddGameObject(JumpWallBarrier3);

	GameObject* JumpWallBarrier4 = CommonUtils::BuildCuboidObject(
		"JumpWallBarrier4",
		Vector3(0.0f, 43.83f, 110.9f),		// Position = Right+---Up+---Foward+
		Vector3(7.0f, 2.0f, 1.0f),			// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpWallBarrier4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-50, Vector3(1, 0, 0))));
	this->AddGameObject(JumpWallBarrier4);

	// Jump Ramps Front
	GameObject* JumpRamp3 = CommonUtils::BuildCuboidObject(
		"JumpRamp3",
		Vector3(0.0f, 17.6f, -91.2f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 8.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp3->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-20, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp3);

	GameObject* JumpRamp4 = CommonUtils::BuildCuboidObject(
		"JumpRamp4",
		Vector3(0.0f, 22.85f, -103.1f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 5.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-30, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp4);

	GameObject* JumpRamp5 = CommonUtils::BuildCuboidObject(
		"JumpRamp5",
		Vector3(0.0f, 27.5f, -109.5f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 3.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp5->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-47, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp5);

	GameObject* JumpRamp6 = CommonUtils::BuildCuboidObject(
		"JumpRamp6",
		Vector3(0.0f, 31.75f, -112.55f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp6->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-65, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp6);

	GameObject* JumpRamp7 = CommonUtils::BuildCuboidObject(
		"JumpRamp7",
		Vector3(0.0f, 34.88f, -113.57f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp7->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-85, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp7);

	// Jump Ramps Back
	GameObject* JumpRamp8 = CommonUtils::BuildCuboidObject(
		"JumpRamp8",
		Vector3(0.0f, 17.6f, 91.2f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 8.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp8->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(20, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp8);

	GameObject* JumpRamp9 = CommonUtils::BuildCuboidObject(
		"JumpRamp9",
		Vector3(0.0f, 22.85f, 103.1f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 5.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp9->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(30, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp9);

	GameObject* JumpRamp10 = CommonUtils::BuildCuboidObject(
		"JumpRamp10",
		Vector3(0.0f, 27.5f, 109.5f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 3.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp10->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(47, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp10);

	GameObject* JumpRamp11 = CommonUtils::BuildCuboidObject(
		"JumpRamp11",
		Vector3(0.0f, 31.75f, 112.55f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 2.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp11->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(65, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp11);

	GameObject* JumpRamp12 = CommonUtils::BuildCuboidObject(
		"JumpRamp12",
		Vector3(0.0f, 34.88f, 113.57f),	// Position = Right+---Up+---Foward+
		Vector3(7.0f, 1.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	JumpRamp12->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(85, Vector3(1, 0, 0))));
	this->AddGameObject(JumpRamp12);

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
}