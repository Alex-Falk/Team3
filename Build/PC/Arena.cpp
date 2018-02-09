#include "Arena.h"
#include <ncltech\PhysicsEngine.h>


void Environment::OnInitializeScene() {

	player = new Player(Vector3(0.0, 1.0, 0.0), DEFAULT, 1.0f);

	this->AddGameObject(player->GetGameObject());
	Environment::CreateEnvironment();


	GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);

	Scene::OnInitializeScene();
}

void Environment::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	player->OnPlayerUpdate(dt);




	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();
}

void Environment::CreateEnvironment()
{
	PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

	//// Create Ground (..everybody loves finding some common ground)
	//GameObject* ground = CommonUtils::BuildCuboidObject(
	//	"Ground",
	//	Vector3(0.0f, 0.0f, 0.0f),			// Centre Position
	//	Vector3(80.0f, 1.5f, 40.0f),		// Scale
	//	true,
	//	0.0f,
	//	true,
	//	false,								// Dragable By User
	//	Vector4(0.2f, 0.5f, 1.0f, 0.0f));	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	//this->AddGameObject(ground);


	//// Create FrontWall (..everybody loves finding some common ground)
	//GameObject* FrontWall = CommonUtils::BuildCuboidObject(
	//	"FrontWall",
	//	Vector3(0.0f, 13.5f, -40.0f),		// Position = Right+---Up+---Foward+
	//	Vector3(80.0f, 12.0f, 1.5f),		// Scale	= Width+---Length+---Depth+
	//	true,
	//	0.0f,
	//	true,
	//	false,								// Dragable By User
	//	Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	//this->AddGameObject(FrontWall);


	//// Create BackWall (..everybody loves finding some common ground)
	//GameObject* BackWall = CommonUtils::BuildCuboidObject(
	//	"BackWall",
	//	Vector3(0.0f, 13.5f, 40.0f),		// Position = Right+---Up+---Foward+
	//	Vector3(80.0f, 12.0f, 1.5f),		// Scale	= Width+---Length+---Depth+
	//	true,
	//	0.0f,
	//	true,
	//	false,								// Dragable By User
	//	Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	//this->AddGameObject(BackWall);


	//// Create LeftWall (..everybody loves finding some common ground)
	//GameObject* LeftWall = CommonUtils::BuildCuboidObject(
	//	"LeftWall",
	//	Vector3(-80.0f, 13.5f, 0.0f),		// Position = Right+---Up+---Foward+
	//	Vector3(1.5f, 12.0f, 40.0f),		// Scale	= Width+---Length+---Depth+
	//	true,
	//	0.0f,
	//	true,
	//	false,								// Dragable By User
	//	Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	//this->AddGameObject(LeftWall);


	//// Create RightWall (..everybody loves finding some common ground)
	//GameObject* RightWall = CommonUtils::BuildCuboidObject(
	//	"RightWall",
	//	Vector3(80.0f, 13.5f, 0.0f),		// Position = Right+---Up+---Foward+
	//	Vector3(1.5f, 12.0f, 40.0f),		// Scale	= Width+---Length+---Depth+
	//	true,
	//	0.0f,
	//	true,
	//	false,								// Dragable By User
	//	Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	//this->AddGameObject(RightWall);

	// Create Ground (..everybody loves finding some common ground)
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Centre Position
		Vector3(55.0f, 1.5f, 30.0f),		// Scale
		true,
		0.0f,
		true,
		false,								// Dragable By User
		Vector4(0.2f, 0.5f, 0.0f, 1.0f));	// Colour       Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	this->AddGameObject(ground);


	// Create FrontWall (..everybody loves finding some common ground)
	GameObject* FrontWall = CommonUtils::BuildCuboidObject(
		"FrontWall",
		Vector3(0.0f, 9.5f, -31.5f),		// Position = Right+---Up+---Foward+
		Vector3(55.0f, 8.0f, 1.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	this->AddGameObject(FrontWall);


	// Create BackWall (..everybody loves finding some common ground)
	GameObject* BackWall = CommonUtils::BuildCuboidObject(
		"BackWall",
		Vector3(0.0f, 9.5f, 31.5f),		// Position = Right+---Up+---Foward+
		Vector3(55.0f, 8.0f, 1.5f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	this->AddGameObject(BackWall);


	// Create LeftWall (..everybody loves finding some common ground)
	GameObject* LeftWall = CommonUtils::BuildCuboidObject(
		"LeftWall",
		Vector3(-56.5f, 9.5f, 0.0f),		// Position = Right+---Up+---Foward+
		Vector3(1.5f, 8.0f, 30.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	this->AddGameObject(LeftWall);

	// Create RightWall (..everybody loves finding some common ground)
	GameObject* RightWall = CommonUtils::BuildCuboidObject(
		"RightWall",
		Vector3(56.5f, 9.5f, 0.0f),		// Position = Right+---Up+---Foward+
		Vector3(1.5f, 8.0f, 30.0f),		// Scale	= Width+---Length+---Depth+
		true,
		0.0f,
		true,
		false,								// Dragable By User
		Vector4(0.2f, 0.5f, 1.0f, 1.0f));	// Colour

	this->AddGameObject(RightWall);
}
