//Name: Nick Bedford
//Date: 06/02/2018
//Added a pickup to test
#pragma once

#include <nclgl\NCLDebug.h>
#include <ncltech\Scene.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include "GamePlay.h"
#include "Player.h"
#include "Pickup.h"

// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions

Player* p1;
Pickup* pickUpOne;

class Blank : public Scene
{
public:
	Blank(const std::string& friendly_name)
		: Scene(friendly_name)
	{
	}

	const Vector3 ss_pos = Vector3(-5.5f, 1.5f, -5.0f);
	const Vector3 sc_pos = Vector3(4.5f, 1.5f, -5.0f);
	const Vector3 cc_pos = Vector3(-0.5f, 1.5f, 5.0f);

	virtual void OnInitializeScene() override
	{
		//Create Ground (..everybody loves finding some common ground)
		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));

		this->AddGameObject(ground);

		p1 = new Player(Vector3(0.0, 1.0, 0.0), GREEN, 1.0f);

		this->AddGameObject(p1->GetObj());
		
		pickUpOne = new Pickup(Vector3(0.0f, 1.0f, 5.0f), SPEED_BOOST);

		this->AddGameObject(pickUpOne->GetObj());

		GraphicsPipeline::Instance()->GetCamera()->SetCenter(p1->GetObj()->Physics());
		GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);


		Scene::OnInitializeScene();
	}

	float m_AccumTime = 0;
	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);
		m_AccumTime += dt;

		p1->Input(dt);
		pickUpOne->Update(dt);



		uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	}
};

