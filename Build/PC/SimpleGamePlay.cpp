#include "WeaponPickup.h"
#include "SimpleGamePlay.h"

void SimpleGamePlay::OnInitializeScene() {

	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	//Create Ground (..everybody loves finding some common ground)
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"BigObject",
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(40.0f, 1.0f, 40.0f),
		true,
		0.0f,
		true,
		false,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f));

	this->AddGameObject(ground);

	player = new Player(Vector3(0.0, 1.0, 0.0), DEFAULT, 0, 1.0f);

	this->AddGameObject(player->GetGameObject());

	pickup = new Pickup(Vector3(0, 3, 0), SPEED_BOOST);

	this->AddGameObject(pickup->GetObj());

	GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);



	Scene::OnInitializeScene();
}

void SimpleGamePlay::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	player->OnPlayerUpdate(dt);

	if (pickup)
	{
		pickup->Update(dt);
	}

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();
}