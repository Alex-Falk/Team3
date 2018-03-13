// Daniel Burns [12/02/2018] Testing Map
#include "Mountains.h"
#include "PaintPool.h"
#include "Game.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//

void Mountains::OnInitializeScene()
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

	pickupSpeedBoost = new Pickup(Vector3(-12, 3, 5), SPEED_BOOST);
	this->AddGameObject(pickupSpeedBoost);

	pickupJumpBoost = new Pickup(Vector3(-12, 3, -5), JUMP_BOOST);
	this->AddGameObject(pickupJumpBoost);

	pickupPool = new PaintPool(Vector3(13, 1.0f, 0), RED); this->AddGameObject(pickupPool);
	pickupPool = new PaintPool(Vector3(-13, 1.0f, 0), GREEN); this->AddGameObject(pickupPool);

	// Score & GUI initialisation -------------------------------------------------------------------------------------------------
	OnInitializeGUI();

	// General Initialization -----------------------------------------------------------------------------------------------------
	xDimension = 162;
	yDimension = 80;
	groundScoreAccuracy = 15;
	Window::GetWindow().LockMouseToWindow(true);
	Map::OnInitializeScene();
}

void Mountains::SetSpawnLocations()
{
	spawnPositions[0] = Vector3(25, 5, 25);
	spawnPositions[1] = Vector3(25, 5, -25);
	spawnPositions[2] = Vector3(-25, 5, 25);
	spawnPositions[3] = Vector3(-25, 5, -25);
}

void Mountains::OnUpdateScene(float dt)
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

void Mountains::CreateEnvironment()
{
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Position = Right+---Up+---Foward+
		Vector3(162.0f, 1.0f, 80.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(ground);

	GameObject* UpperFrontLevel = CommonUtils::BuildCuboidObject(
		"UpperFrontLevel",
		Vector3(0, 13.55f, -143.2f),		// Position = Right+---Up+---Foward+
		Vector3(120.0f, 0.1f, 36.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(UpperFrontLevel);

	GameObject* Ramp4 = CommonUtils::BuildCuboidObject(
		"Ramp4",
		Vector3(102.3f, 6.35f, -93.9),	// Position = Right+---Up+---Foward+
		Vector3(27.3f, 1.0f, 15.2f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
	this->AddGameObject(Ramp4);

	GameObject* Ramp6 = CommonUtils::BuildCuboidObject(
		"Ramp6",
		Vector3(-102.3f, 6.35f, -93.9f),	// Position = Right+---Up+---Foward+
		Vector3(27.3f, 1.0f, 15.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	Ramp6->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
	this->AddGameObject(Ramp6);

	GameObject* Cliff3 = CommonUtils::BuildCuboidObject(
		"CliffSouth",
		Vector3(0.0f, 7.25f, -93.6f),	// Position = Right+---Up+---Foward+
		Vector3(75.0f, 6.4f, 13.6f),	// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(Cliff3);

	GameObject* BackWall2 = CommonUtils::BuildCuboidObject(
		"BackWall2",
		Vector3(0, 26.55f, -180.0f),		// Position = Right+---Up+---Foward+
		Vector3(60.0f, 13.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(BackWall2);
 
	GameObject* FrontWall = CommonUtils::BuildCuboidObject(
		"FrontWall",
		Vector3(0.0f, 20.3f, 80.0f),		// Position = Right+---Up+---Foward+
		Vector3(162.2f, 19.3f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(FrontWall);

	GameObject* SideWall = CommonUtils::BuildCuboidObject(
		"SideWall",
		Vector3(107.8f, 20.3f, -112.4f),		// Position = Right+---Up+---Foward+
		Vector3(82.0f, 19.3f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	SideWall->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(55, Vector3(0, 1, 0))));
	this->AddGameObject(SideWall);

	GameObject* SideWall2 = CommonUtils::BuildCuboidObject(
		"SideWall2",
		Vector3(-107.8f, 20.3f, -112.4f),		// Position = Right+---Up+---Foward+
		Vector3(82.0f, 19.3f, 1.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	SideWall2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-55, Vector3(0, 1, 0))));
	this->AddGameObject(SideWall2);

	GameObject* SideWall3 = CommonUtils::BuildCuboidObject(
		"SideWall3",
		Vector3(155.0f, 20.3f, 17.7f),		// Position = Right+---Up+---Foward+
		Vector3(1.0f, 19.3f, 62.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(SideWall3);

	GameObject* SideWall4 = CommonUtils::BuildCuboidObject(
		"SideWall4",
		Vector3(-155.0f, 20.3f, 17.7f),		// Position = Right+---Up+---Foward+
		Vector3(1.0f, 19.3f, 62.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(SideWall4);
}
