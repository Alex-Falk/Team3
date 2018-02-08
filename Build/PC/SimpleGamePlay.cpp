#include "WeaponPickup.h"
#include "SimpleGamePlay.h"

void SimpleGamePlay::OnInitializeScene() {
	//Create Ground (..everybody loves finding some common ground)
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(40.0f, 1.0f, 40.0f),
		true,
		0.0f,
		true,
		false,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f));

	this->AddGameObject(ground);

	player = new Player(Vector3(0.0, 1.0, 0.0), DEFAULT, 1.0f);

	this->AddGameObject(player->GetGameObject());

	pickup = new WeaponPickup(Vector3(0.0f, 3.0f, 0.0f), PAINT_ROCKET, 3.0f);
	
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

	pickup->Update(dt);


	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();
}