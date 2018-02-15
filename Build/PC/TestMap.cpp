// Daniel Burns [12/02/2018] Testing Map

#include <ncltech\GameObject.h>
#include <ncltech\CommonUtils.h>
#include <nclgl\NCLDebug.h>
#include "GamePlay.h"
#include "PaintableGameObject.h"
#include "TestMap.h"
#include "Pickup.h"
#include <string.h>
#include "Avatar.h"
#include "SimpleGamePlay.h"

void TestMap::OnInitializeScene()
{
	GraphicsPipeline::Instance()->SetIsMainMenu(false);
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");
	
	pickup_SpeedBoost = new Pickup(Vector3(5, 3, 4.5), SPEED_BOOST);
	this->AddGameObject(pickup_SpeedBoost);

	pickup_JumpBoost = new Pickup(Vector3(-7, 3, -1), JUMP_BOOST);
	this->AddGameObject(pickup_JumpBoost);
	
	pickup_Weapon = new Pickup(Vector3(1, 3, -5.5), WEAPON);
	this->AddGameObject(pickup_Weapon);
	
	TestMap::CreateEnvironment();	// creates environment & elements within.
	TestMap::GameplayTesting();	   // gameplay functionality testing.
	TestMap::PaintEnvironment();  // paints objects in level.

	GraphicsPipeline::Instance()->InitPath(Vector2(30, 30));

	OnInitializeGUI();
	Scene::OnInitializeScene();
}

void TestMap::onConnectToScene()
{
	for (uint i = 0; i < 4; i++) {
		if (Game::Instance()->GetUser())
		{
			Avatar * p = nullptr;
			if (i == Game::Instance()->getUserID())
			{
				p = new ControllableAvatar(Vector3(i * 3, 1.0, 0.0), Colour(i), i, 1.0f);
			}
			else
			{
				p = new Avatar(Vector3(i * 3, 1.0, 0.0), Colour(i), i, 1.0f);
			}

			this->AddGameObject(p->GetGameObject());
			Game::Instance()->SetAvatar(i, p);

			GraphicsPipeline::Instance()->AddPlayerRenderNode(Game::Instance()->GetPlayer(i)->GetGameObject()->Render());
		}
	}
}

void TestMap::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	for (uint i = 0; i < 4; i++) {
		if (Game::Instance()->GetPlayer(i))
			Game::Instance()->GetPlayer(i)->OnAvatarUpdate(dt);
	}

	if (pickup_JumpBoost)
	{
		pickup_JumpBoost->Update(dt);
	}

	if (pickup_SpeedBoost)
	{
		pickup_SpeedBoost->Update(dt);
	}

	if (pickup_Weapon)
	{
		pickup_Weapon->Update(dt);
	}

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	if (Game::Instance()->GetUser())
	{
		if (Game::Instance()->GetPlayer(Game::Instance()->getUserID()))
			energyBar->setProgress(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetLife() / 100.0f);
	}

	Vector3 pickup_SpeedBoost = Vector3(0.0, 2.0, 0.0);
	Vector3 pickup_JumpBoost = Vector3(0.0, 2.0, 0.0);
	Vector3 pickup_Weapon = Vector3(0.0, 2.0, 0.0);

	NCLDebug::DrawTextWs(pickup_SpeedBoost + Vector3(5.0f, 3.2f, 4.5f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "SPEED");
	NCLDebug::DrawTextWs(pickup_JumpBoost + Vector3(-7.0f, 3.2f, -1.f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "JUMP");
	NCLDebug::DrawTextWs(pickup_Weapon + Vector3(1.0f, 3.2f, -5.5f), STATUS_TEXT_SIZE, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), "WEAPON");
}

void TestMap::OnInitializeGUI()
{
	//Call initi-function for gui
	sceneGUI = new GUI();
	sceneGUI->Init(CEGUIDIR);

	//Load Scheme - Which actually means UI style - notice that multiple Scheme could be load at once
	sceneGUI->LoadScheme("TaharezLook.scheme");
	sceneGUI->LoadScheme("AlfiskoSkin.scheme");

	//Set Font sytle
	sceneGUI->SetFont("DejaVuSans-10");

	//SetMouseCursor
	sceneGUI->SetMouseCursor("TaharezLook/MouseArrow");
	sceneGUI->ShowMouseCursor();

	//Create Push Button handle
	energyBar = static_cast<CEGUI::ProgressBar*>(
		sceneGUI->createWidget("TaharezLook/ProgressBar",
			Vector4(0.40f, 0.9f, 0.2f, 0.03f),
			Vector4(),
			"energyBar"
		));

	if (Game::Instance()->GetUser())
	{
		if (Game::Instance()->GetPlayer(Game::Instance()->getUserID()))
			energyBar->setProgress(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetLife() / 100.0f);
	}
}

void TestMap::OnCleanUpScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoveAllTexture();
	GraphicsPipeline::Instance()->RemoveAllPlayerRenderNode();
}

void TestMap::CreateEnvironment()
{
	//PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

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
	active = true;
	respawnTime = 30.0;
	currentRespawnTimer = 0.0;

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

void TestMap::PaintEnvironment()
{
	//player->GetColour() == colour;

	//switch (player->GetColour())
	//{
	//case RED:
	//	object->Render()->GetChild()->SetBaseColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	//	colour = RED;
	//	break;

	//case GREEN:
	//	object->Render()->GetChild()->SetBaseColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//	colour = GREEN;
	//	break;

	//case BLUE:
	//	object->Render()->GetChild()->SetBaseColor(Vector4(0.f, 0.0f, 1.0f, 1.0f));
	//	colour = BLUE;
	//	break;

	//case PINK:
	//	object->Render()->GetChild()->SetBaseColor(Vector4(1.2f, 0.5f, 1.0f, 1.0f));
	//	colour = PINK;
	//	break;
	//}
}

void TestMap::OnButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}
