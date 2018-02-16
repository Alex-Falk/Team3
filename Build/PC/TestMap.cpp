// Daniel Burns [12/02/2018] Testing Map
#include "TestMap.h"
#include "PaintPool.h"
#include "Game.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void TestMap::OnInitializeScene()
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
	
	// Spawning Pickups -----------------------------------------------------------------------------------------------------------
	pickupSpeedBoost = new Pickup(Vector3(12, 3, 5.5), SPEED_BOOST);
	this->AddGameObject(pickupSpeedBoost);

	pickupJumpBoost = new Pickup(Vector3(-12, 3, 0), JUMP_BOOST);
	this->AddGameObject(pickupJumpBoost);
	
	pickupWeapon = new Pickup(Vector3(12, 3, -5.5), WEAPON);
	this->AddGameObject(pickupWeapon);

	pickupPool = new PaintPool(Vector3(0, 1.1f, 0), RED);
	this->AddGameObject(pickupPool);
	
	CreateEnvironment();	// creates environment & elements within.
	GameplayTesting();		// gameplay functionality testing.

	GraphicsPipeline::Instance()->InitPath(Vector2(xDimension, yDimension));
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(40);			
	GraphicsPipeline::Instance()->InitPath(Vector2(40, 40));							

	// Score & GUI initialisation -------------------------------------------------------------------------------------------------
	BuildGroundScore();
	OnInitializeGUI();

	// General Initialization -----------------------------------------------------------------------------------------------------
	Map::OnInitializeScene();
}

void TestMap::OnCleanUpScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoveAllTexture();
	GraphicsPipeline::Instance()->RemoveAllPlayerRenderNode();
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

	if (pickupWeapon)
	{
		pickupWeapon->Update(dt);
	}

	// Display Pickup Text ------------------------------------------------------------------------------------------------------
	Vector3 pickupSpeedBoostText = Vector3(0.0, 2.0, 0.0);
	Vector3 pickupJumpBoostText = Vector3(0.0, 2.0, 0.0);
	Vector3 pickupWeaponText = Vector3(0.0, 2.0, 0.0);

	NCLDebug::DrawTextWs(pickupSpeedBoostText + Vector3(12.0f, 3.0f, 5.5f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "SPEED");
	NCLDebug::DrawTextWs(pickupJumpBoostText + Vector3(-12.0f, 3.0, 0.f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "JUMP");
	NCLDebug::DrawTextWs(pickupWeaponText + Vector3(12.0f, 3.0f, -5.5f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "WEAPON");
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

//--------------------------------------------------------------------------------------------//
// Score Related Functions
//--------------------------------------------------------------------------------------------//
// These two need to be in each scene because they are dependent on the array which is defined in the scene itself

void TestMap::BuildGroundScore() {


	for (int i = 0; i < xOnGrid - 1; i++) {
		for (int j = 0; j < yOnGrid - 1; j++) {
			ground[i][j] = START_COLOUR;
		}
	}

	ground[0][0] = RED;
	ground[0][yOnGrid - 1] = GREEN;
	ground[xOnGrid - 1][0] = BLUE;
	ground[xOnGrid - 1][yOnGrid - 1] = PINK;
	for (int i = 0; i < 4; i++)
	{
		groundTeamScore[i] = 0;
	}
}

void TestMap::UpdateGroundScore(Avatar* player) {

	Vector2 playerPos = Vector2((player->GetPosition().x * groundScoreAccuracy) + xOnGrid / 2, (player->GetPosition().z * groundScoreAccuracy) + yOnGrid / 2);
	int plGridSize = (int)(player->GetLife() * groundScoreAccuracy / 100);

	// Runs through the square arount the center and finds the circle.
	for (int i = playerPos.x - plGridSize; i <= playerPos.x; i++) {
		for (int j = playerPos.y - plGridSize; j <= playerPos.y; j++) {
			if ((i - playerPos.x)*(i - playerPos.x) + (j - playerPos.y)* (j - playerPos.y) <= plGridSize * plGridSize) {
				int xSym = playerPos.x - (i - playerPos.x);
				int ySym = playerPos.y - (i - playerPos.y);
				// Thanks to symetry we take all 4 quadrants of the circle arount the center
				ChangeGridScore(ground[i][j], player->GetColour());
				ground[i][j] = player->GetColour();
				ChangeGridScore(ground[i][ySym], player->GetColour());
				ground[i][ySym] = player->GetColour();
				ChangeGridScore(ground[xSym][j], player->GetColour());
				ground[xSym][j] = player->GetColour();
				ChangeGridScore(ground[xSym][ySym], player->GetColour());
				ground[xSym][ySym] = player->GetColour();
			}
		}
	}
}