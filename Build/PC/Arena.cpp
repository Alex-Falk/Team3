// Daniel Burns [12/02/2018] Testing Map
#include "Arena.h"
#include "PaintPool.h"
#include "Game.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//

void level::OnInitializeScene()
{
	spawnPositions[0] = Vector3(25, 5, 25);
	spawnPositions[1] = Vector3(25, 5, -25);
	spawnPositions[2] = Vector3(-25, 5, 25);
	spawnPositions[3] = Vector3(-25, 5, -25);

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

	pickupPool = new PaintPool(Vector3(13, 1.0f, 0), RED); this->AddGameObject(pickupPool);
	pickupPool = new PaintPool(Vector3(-13, 1.0f, 0), GREEN); this->AddGameObject(pickupPool);

	// Score & GUI initialisation -------------------------------------------------------------------------------------------------
	OnInitializeGUI();

	// General Initialization -----------------------------------------------------------------------------------------------------
	xDimension = 100;
	yDimension = 100;
	groundScoreAccuracy = 15;
	Window::GetWindow().LockMouseToWindow(true);
	Map::OnInitializeScene();
}

void level::SetSpawnLocations()
{
	//spawnPositions[0] = Vector3(25, 5, 25);
	//spawnPositions[1] = Vector3(25, 5, -25);
	//spawnPositions[2] = Vector3(-25, 5, 25);
	//spawnPositions[3] = Vector3(-25, 5, -25);
}

void level::OnUpdateScene(float dt)
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
}

void level::CreateEnvironment()
{
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Position = Right+---Up+---Foward+
		Vector3(100.0f, 1.0f, 100.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(ground);

	{
		//// ----------- Right Side --------------------------------------------------------------------------------------
		GameObject* UpperLevel = CommonUtils::BuildCuboidObject(
			"UpperLevel1",
			Vector3(157.2f, 13.55f, 0.0f),		// Position = Right+---Up+---Foward+
			Vector3(30.0f, 0.1f, 100.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(UpperLevel);

		//// Cliff 1 (..everybody loves finding some common ground)
		GameObject* Cliff1 = CommonUtils::BuildCuboidObject(
			"CliffSouth",
			Vector3(113.6f, 7.25f, 0.0f),	// Position = Right+---Up+---Foward+
			Vector3(13.6f, 6.4f, 75.0f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Cliff1);

		//// Ramp 1 (..everybody loves finding some common ground)
		GameObject* Ramp1 = CommonUtils::BuildCuboidObject(
			"Ramp1",
			Vector3(114.0f, 6.4f, -87.5f),	// Position = Right+---Up+---Foward+
			Vector3(15.0f, 1.0f, 12.5f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp1->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(0, 0, 1))));
		this->AddGameObject(Ramp1);

		// Ramp 2 (..everybody loves finding some common ground)
		GameObject* Ramp2 = CommonUtils::BuildCuboidObject(
			"Ramp2",
			Vector3(114.0f, 6.4f, 87.5f),	// Position = Right+---Up+---Foward+
			Vector3(15.0f, 1.0f, 12.5f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(0, 0, 1))));
		this->AddGameObject(Ramp2);
	}
	{
		// ----------- Left Side --------------------------------------------------------------------------------------
		GameObject* UpperLevel1 = CommonUtils::BuildCuboidObject(
			"UpperLevel1",
			Vector3(-157.2f, 13.55f, 0.0f),		// Position = Right+---Up+---Foward+
			Vector3(30.0f, 0.1f, 100.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(UpperLevel1);

		// Cliff 1 (..everybody loves finding some common ground)
		GameObject* Cliff = CommonUtils::BuildCuboidObject(
			"CliffSouth",
			Vector3(-113.6f, 7.25f, 0.0f),	// Position = Right+---Up+---Foward+
			Vector3(13.6f, 6.4f, 75.0f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Cliff);

		// Ramp 1 (..everybody loves finding some common ground)
		GameObject* Ramp = CommonUtils::BuildCuboidObject(
			"Ramp",
			Vector3(-114.0f, 6.4f, -87.5f),	// Position = Right+---Up+---Foward+
			Vector3(15.0f, 1.0f, 12.5f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(25, Vector3(0, 0, 1))));
		this->AddGameObject(Ramp);

		// Ramp 2 (..everybody loves finding some common ground)
		GameObject* Ramp0 = CommonUtils::BuildCuboidObject(
			"Ramp0",
			Vector3(-114.0f, 6.4f, 87.5f),	// Position = Right+---Up+---Foward+
			Vector3(15.0f, 1.0f, 12.5f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp0->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(25, Vector3(0, 0, 1))));
		this->AddGameObject(Ramp0);
	}
	{
		// ----------- Front Side --------------------------------------------------------------------------------------
		GameObject* UpperFrontLevel = CommonUtils::BuildCuboidObject(
			"UpperFrontLevel",
			Vector3(0, 13.55f, -157.2f),		// Position = Right+---Up+---Foward+
			Vector3(100.0f, 0.1f, 30.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(UpperFrontLevel);

		// Cliff 1 (..everybody loves finding some common ground)
		GameObject* Cliff3 = CommonUtils::BuildCuboidObject(
			"CliffSouth",
			Vector3(0.0f, 7.25f, -113.6f),	// Position = Right+---Up+---Foward+
			Vector3(75.0f, 6.4f, 13.6f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Cliff3);

		// Ramp 1 (..everybody loves finding some common ground)
		GameObject* Ramp4 = CommonUtils::BuildCuboidObject(
			"Ramp4",
			Vector3(87.5f, 6.4f, -114.0f),	// Position = Right+---Up+---Foward+
			Vector3(12.5f, 1.0f, 15.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp4);

		// Ramp 2 (..everybody loves finding some common ground)
		GameObject* Ramp5 = CommonUtils::BuildCuboidObject(
			"Ramp5",
			Vector3(-87.5f, 6.4f, -114.0f),	// Position = Right+---Up+---Foward+
			Vector3(12.5f, 1.0f, 15.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp5->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp5);
	}
	{
		// ----------- Back Side --------------------------------------------------------------------------------------
		GameObject* UpperBackLevel = CommonUtils::BuildCuboidObject(
			"UpperFrontLevel",
			Vector3(0, 13.55f, 157.2f),		// Position = Right+---Up+---Foward+
			Vector3(100.0f, 0.1f, 30.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(UpperBackLevel);

		// Cliff 1 (..everybody loves finding some common ground)
		GameObject* Cliff4 = CommonUtils::BuildCuboidObject(
			"CliffSouth",
			Vector3(0.0f, 7.25f, 113.6f),	// Position = Right+---Up+---Foward+
			Vector3(75.0f, 6.4f, 13.6f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Cliff4);

		// Ramp 1 (..everybody loves finding some common ground)
		GameObject* Ramp5 = CommonUtils::BuildCuboidObject(
			"Ramp5",
			Vector3(87.5f, 6.4f, 114.0f),	// Position = Right+---Up+---Foward+
			Vector3(12.5f, 1.0f, 15.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp5->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp5);

		// Ramp 2 (..everybody loves finding some common ground)
		GameObject* Ramp6 = CommonUtils::BuildCuboidObject(
			"Ramp6",
			Vector3(-87.5f, 6.4f, 114.0f),	// Position = Right+---Up+---Foward+
			Vector3(12.5f, 1.0f, 15.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp6->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp6);
	}
	{
		// Back Walls
		GameObject* BackWall = CommonUtils::BuildCuboidObject(
			"BackWall",
			Vector3(0, 26.55f, 188.2f),		// Position = Right+---Up+---Foward+
			Vector3(100.0f, 13.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(BackWall);

		GameObject* BackWall2 = CommonUtils::BuildCuboidObject(
			"BackWall2",
			Vector3(0, 26.55f, -188.2f),		// Position = Right+---Up+---Foward+
			Vector3(100.0f, 13.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(BackWall2);

		GameObject* BackWall3 = CommonUtils::BuildCuboidObject(
			"BackWall3",
			Vector3(-188.2f, 26.55f, 0.0f),		// Position = Right+---Up+---Foward+
			Vector3(1.0f, 13.0f, 100.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(BackWall3);

		GameObject* BackWall4 = CommonUtils::BuildCuboidObject(
			"BackWall4",
			Vector3(188.2f, 26.55f, 0.0f),		// Position = Right+---Up+---Foward+
			Vector3(1.0f, 13.0f, 100.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(BackWall4);
	}
	{
		// Side Walls
		GameObject* SideWall = CommonUtils::BuildCuboidObject(
			"SideWall",
			Vector3(-144.1, 26.55f, 144.5f),	// Position = Right+---Up+---Foward+
			Vector3(62.0f, 13.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		SideWall->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(45, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall);

		GameObject* SideWall2 = CommonUtils::BuildCuboidObject(
			"SideWall2",
			Vector3(144.1, 26.55f, -144.5f),		// Position = Right+---Up+---Foward+
			Vector3(62.0f, 13.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		SideWall2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(45, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall2);

		GameObject* SideWall3 = CommonUtils::BuildCuboidObject(
			"SideWall3",
			Vector3(144.1, 26.55f, 144.5f),		// Position = Right+---Up+---Foward+
			Vector3(62.0f, 13.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		SideWall3->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-45, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall3);

		GameObject* SideWall4 = CommonUtils::BuildCuboidObject(
			"SideWall4",
			Vector3(-144.1, 26.55f, -144.5f),		// Position = Right+---Up+---Foward+
			Vector3(62.0f, 13.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour
			MATERIALTYPE::Ground);
		SideWall4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-45, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall4);
	}
	{
		// Corner Blocks
		GameObject* Block = CommonUtils::BuildCuboidObject(
			"Block",
			Vector3(-143.52f, 7.25f, 143.52f),	// Position = Right+---Up+---Foward+
			Vector3(43.52f, 6.4f, 43.52f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Block);

		GameObject* Block2 = CommonUtils::BuildCuboidObject(
			"Block2",
			Vector3(-143.52f, 7.25f, -143.52f),	// Position = Right+---Up+---Foward+
			Vector3(43.52f, 6.4f, 43.52f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Block2);

		GameObject* Block3 = CommonUtils::BuildCuboidObject(
			"Block3",
			Vector3(143.52f, 7.25f, -143.52f),	// Position = Right+---Up+---Foward+
			Vector3(43.52f, 6.4f, 43.52f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Block3);

		GameObject* Block4 = CommonUtils::BuildCuboidObject(
			"Block4",
			Vector3(143.52f, 7.25f, 143.52f),	// Position = Right+---Up+---Foward+
			Vector3(43.52f, 6.4f, 43.52f),	// Scale	= Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,								// Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),	// Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Block4);
	}
}
