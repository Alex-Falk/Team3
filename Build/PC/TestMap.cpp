// Daniel Burns [12/02/2018] Testing Map
#include "TestMap.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void TestMap::OnInitializeScene()
{
	dimensions = Vector2(30,30);

	Map::SetMapDimensions(dimensions);
	Map::OnInitializeScene();
}

void TestMap::SetSpawnLocations()
{
	spawnPositions[0] = Vector3(20, 5, 20);
	spawnPositions[1] = Vector3(20, 5, -20);
	spawnPositions[2] = Vector3(-20, 5, 20);
	spawnPositions[3] = Vector3(-20, 5, -20);

}



//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void TestMap::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	for (uint i = 0; i < npickup; ++i)
	{
		if (pickup[i])
		{
			pickup[i]->Update(dt);
		}
	}

	NCLDebug::DrawTextWs(pickupTextOffset[0] + pickup[0]->Physics()->GetPosition(), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "SPEED");
	NCLDebug::DrawTextWs(pickupTextOffset[1] + pickup[1]->Physics()->GetPosition(), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "JUMP");
	NCLDebug::DrawTextWs(pickupTextOffset[2] + pickup[2]->Physics()->GetPosition(), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "WEAPON");
}

void TestMap::AddObjects()
{
	pickup[0] = new Pickup(Vector3(5, 3, 4.5), SPEED_BOOST);
	this->AddGameObject(pickup[0]);
	pickupTextOffset[0] = Vector3(0, 2, 0);

	pickup[1] = new Pickup(Vector3(-7, 3, -1), JUMP_BOOST);
	this->AddGameObject(pickup[1]);
	pickupTextOffset[1] = Vector3(0, 2, 0);

	pickup[2] = new Pickup(Vector3(1, 3, -5.5), WEAPON);
	this->AddGameObject(pickup[2]);
	pickupTextOffset[2] = Vector3(0, 2, 0);

	CreateEnvironment();
	GameplayTesting();
}


void TestMap::CreateEnvironment()
{
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Centre Position
		Vector3(30.0f, 1.5f, 30.0f),		// Scale
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
		Vector3(0.0f, 2.0f, -31.0f),	// Position = Right+---Up+---Foward+
		Vector3(30.0f, 1.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
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
		Vector3(0.0f, 2.0f, 31.0f),		// Position = Right+---Up+---Foward+
		Vector3(30.0f, 1.0f, 1.0f),		// Scale	= Width+---Length+---Depth+
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
		Vector3(-31.0f, 2.0f, 0.0f),	// Position = Right+---Up+---Foward+
		Vector3(1.0f, 1.0f, 30.0f),		// Scale	= Width+---Length+---Depth+
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
		Vector3(31.0f, 2.0f, 0.0f),		// Position = Right+---Up+---Foward+
		Vector3(1.0f, 1.0f, 30.0f),		// Scale	= Width+---Length+---Depth+
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
		Vector3(-16.0f, 2.5f, -16.0f),
		Vector3(3.5f, 1.0f, 3.5f),
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PhysNodeType::DEFAULT_PHYSICS,
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),	    //Colour	
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(object);

	object = CommonUtils::BuildCuboidObject("PaintableObject2",
		Vector3(14.0f, 3.0f, 12.0f),
		Vector3(1.5f, 1.5f, 1.5f),
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PhysNodeType::DEFAULT_PHYSICS,
		Vector4(1.0f, 1.0f, 1.0f, 0.5f),	    //Colour	
		MATERIALTYPE::Forward_Lighting);
	this->AddGameObject(object);
}
