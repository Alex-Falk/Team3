#include "WeaponPickup.h"
#include "MapOne.h"
#include "PaintPool.h"
#include "Game.h"
#include "MultiPaintPool.h"
#include "CaptureArea.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void MapOne::OnInitializeScene()
{
	dimensions = Vector2(162,80);

	Map::SetMapDimensions(dimensions);
	PhysicsEngine::Instance()->SetLimits(Vector3(-dimensions.x, -20, -dimensions.y), Vector3(dimensions.x, 50, dimensions.y));
	Map::OnInitializeScene();
}


void MapOne::SetSpawnLocations()
{
	GUIsystem::Instance()->SetDrawScoreBar(true);
	spawnPositions[0] = Vector3(35, 5, 35);
	spawnPositions[1] = Vector3(35, 5, -35);
	spawnPositions[2] = Vector3(-35, 5, 35);
	spawnPositions[3] = Vector3(-35, 5, -35);
}

void MapOne::AddObjects()
{
	CreateEnvironment();
	//BuildGround(dimensions);

	//// PICKUPS 
	//AddPickup(new PaintPool(Vector3(0, 0.6f, 0), RED, "0"));
	//AddPickup(new WeaponPickup(Vector3(20, 1.5, 20), PAINT_SPRAY, "1", 5.0f));
	//AddPickup(new Pickup(Vector3(-10, 1.5, 7), PickupType::SPEED_BOOST, "2"));
	//AddPickup(new Pickup(Vector3(2, 1.5, -7), PickupType::JUMP_BOOST, "3"));
	//AddPickup(new PaintPool(Vector3(-15.0f, 0.6f, -15.0f), GREEN, "4"));


	//// CAPTUREAREAS
	//AddCaptureArea(new MinionCaptureArea(START_COLOUR, "0", { 0,1.5f,15 }, { 0.5f,0.5f,0.5f }, 10));
	//AddCaptureArea(new MinionCaptureArea(START_COLOUR, "1", { 0,1.5f,-30 }, { 0.5f,0.5f,0.5f }, 10));
	//AddCaptureArea(new CaptureArea(Vector3(15, 0.6, -15), "2", Vector3(3.0f, 0.5f, 3.0f), 10));

	
	PhysicsEngine::Instance()->ResetWorldPartition();
}

//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void MapOne::OnUpdateScene(float dt)
{
	perfMapObjects.UpdateRealElapsedTime(dt);
	Map::OnUpdateScene(dt);

}

//--------------------------------------------------------------------------------------------//
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void MapOne::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}



void MapOne::CreateEnvironment()
{
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),      // Position = Right+---Up+---Foward+
		Vector3(162.0f, 1.0f, 80.0f),    // Scale  = Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,                // Dragable By User
		PhysNodeType::BIG_NODE,
		Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
		MATERIALTYPE::Ground);
	this->AddGameObject(ground);

	{ // Upper Level Code
		GameObject* UpperFrontLevel = CommonUtils::BuildCuboidObject(
			"UpperFrontLevel",
			Vector3(0, 13.55f, -143.2f),    // Position = Right+---Up+---Foward+
			Vector3(120.0f, 0.1f, 36.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(UpperFrontLevel);

		GameObject* Ramp4 = CommonUtils::BuildCuboidObject(
			"Ramp4",
			Vector3(102.3f, 6.35f, -93.9),  // Position = Right+---Up+---Foward+
			Vector3(27.3f, 1.0f, 15.2f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),  // Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp4->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp4);

		GameObject* Ramp6 = CommonUtils::BuildCuboidObject(
			"Ramp6",
			Vector3(-102.3f, 6.35f, -93.9f),  // Position = Right+---Up+---Foward+
			Vector3(27.3f, 1.0f, 15.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),  // Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp6->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp6);

		GameObject* Cliff3 = CommonUtils::BuildCuboidObject(
			"CliffSouth",
			Vector3(0.0f, 7.25f, -93.6f),  // Position = Right+---Up+---Foward+
			Vector3(75.0f, 6.4f, 13.6f),  // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),  // Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Cliff3);

		GameObject* BackWall2 = CommonUtils::BuildCuboidObject(
			"BackWall2",
			Vector3(0, 26.55f, -180.0f),    // Position = Right+---Up+---Foward+
			Vector3(60.0f, 13.0f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(BackWall2);
	} // Upper Level Code

	{// Lower Level Code
		GameObject* Cliff4 = CommonUtils::BuildCuboidObject(
			"CliffNorth",
			Vector3(0.0f, -5.4f, 93.6f),  // Position = Right+---Up+---Foward+
			Vector3(110.0f, 6.4f, 13.6f),  // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),  // Colour
			MATERIALTYPE::Forward_Lighting);
		this->AddGameObject(Cliff4);

		GameObject* Ramp5 = CommonUtils::BuildCuboidObject(
			"Ramp5",
			Vector3(140.0f, -6.25f, 93.15f),  // Position = Right+---Up+---Foward+
			Vector3(40.0f, 1.0f, 15.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),  // Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp5->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp5);

		GameObject* Ramp7 = CommonUtils::BuildCuboidObject(
			"Ramp7",
			Vector3(-140.0f, -6.25f, 93.15f),  // Position = Right+---Up+---Foward+
			Vector3(30.0f, 1.0f, 15.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),  // Colour
			MATERIALTYPE::Forward_Lighting);
		Ramp7->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-25, Vector3(1, 0, 0))));
		this->AddGameObject(Ramp7);

		GameObject* LowerLevel = CommonUtils::BuildCuboidObject(
			"LowerLevel",
			Vector3(0, -11.8f, 192.1f),    // Position = Right+---Up+---Foward+
			Vector3(200.0f, 0.1f, 85.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(LowerLevel);

		GameObject* FrontWall = CommonUtils::BuildCuboidObject(
			"FrontWall",
			Vector3(0.0f, 13.9f, 277.10f),    // Position = Right+---Up+---Foward+
			Vector3(87.55f, 25.7f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(FrontWall);
  }

	// Side Walls
	{
		GameObject* SideWall = CommonUtils::BuildCuboidObject(
			"SideWall",
			Vector3(107.8f, 20.3f, -112.4f),    // Position = Right+---Up+---Foward+
			Vector3(82.0f, 19.3f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		SideWall->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(55, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall);

		GameObject* SideWall2 = CommonUtils::BuildCuboidObject(
			"SideWall2",
			Vector3(-107.8f, 20.3f, -112.4f),    // Position = Right+---Up+---Foward+
			Vector3(82.0f, 19.3f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		SideWall2->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-55, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall2);

		GameObject* SideWall3 = CommonUtils::BuildCuboidObject(
			"SideWall3",
			Vector3(155.0f, 20.3f, 5.2f),    // Position = Right+---Up+---Foward+
			Vector3(1.0f, 19.3f, 50.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(SideWall3);

		GameObject* SideWall4 = CommonUtils::BuildCuboidObject(
			"SideWall4",
			Vector3(-155.0f, 20.3f, 5.2f),    // Position = Right+---Up+---Foward+
			Vector3(1.0f, 19.3f, 50.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		this->AddGameObject(SideWall4);

		GameObject* SideWall5 = CommonUtils::BuildCuboidObject(
			"SideWall5",
			Vector3(176.2f, 13.9f, 134.1f),    // Position = Right+---Up+---Foward+
			Vector3(82.0f, 25.7f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		SideWall5->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(75, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall5);

		GameObject* SideWall6 = CommonUtils::BuildCuboidObject(
			"SideWall6",
			Vector3(-176.2f, 13.9f, 134.1f),    // Position = Right+---Up+---Foward+
			Vector3(82.0f, 25.7f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 0.5f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		SideWall6->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-75, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall6);

		GameObject* SideWall7 = CommonUtils::BuildCuboidObject(
			"SideWall7",
			Vector3(-142.5f, 13.9f, 245.5f),  // Position = Right+---Up+---Foward+
			Vector3(63.0f, 25.7f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 1.0f, 1.0f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		SideWall7->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(30, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall7);

		GameObject* SideWall8 = CommonUtils::BuildCuboidObject(
			"SideWall8",
			Vector3(142.5f, 13.9f, 245.5f),  // Position = Right+---Up+---Foward+
			Vector3(63.0f, 25.7f, 1.0f),    // Scale  = Width+---Length+---Depth+
			true,
			0.0f,
			true,
			false,                // Dragable By User
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 1.0f, 1.0f, 1.0f),  // Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));  // Colour
			MATERIALTYPE::Ground);
		SideWall8->Physics()->SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-30, Vector3(0, 1, 0))));
		this->AddGameObject(SideWall8);
	}
}